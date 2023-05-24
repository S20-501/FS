#include <sstream>
#include <cstring>
#include <iostream>

#include "Enter.h"
#include "exceptions/FileCannotCreate.hpp"
#include "CommonFunctions.h"
#include "UtilsFunctions.hpp"

std::string Enter::getQuery(){
    return "enter";
}

std::string Enter::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setLength(parser.getKeyArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setFilename(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Enter::checkAmount(const Parser &parser) {
    if(parser.getKeyArgs().size() + parser.getKeyArgs().size() < 2){
        return NOT_ENOUGH_ARGS;
    }
    if(parser.getKeyArgs().size() + parser.getKeyArgs().size() > 2){
        return TOO_MANY_ARGS;
    }

    return "";
}

std::string Enter::setLength(const keyArgs_t &keys) {
    if(auto it = keys.find("length"); it != keys.end() || ((it = keys.find("l")) != keys.end())) {
        // convert to int
        if(UtilsFunctions::convertToNumber(it->second, length)) return LENGTH_CANT_CONVERT;
        // check restrictions
        if(length < 1 || 65535 < length){
            return LENGTH_RESTRICTED;
        }
    } else {
        return NO_LENGTH_VALUE;
    }

    return "";
}

std::string Enter::setFilename(posArgs_t &poss) {
    if (poss.empty()) {
        return NO_FILENAME_VALUE;
    }

    filename = std::move(poss.back());
    poss.pop_back();
    if(filename.size() >= 10){
        return FILENAME_TOO_LONG;
    }

    if(!MonCom::isASCII(filename)){
        return FILENAME_INCORRECT;
    }

    if(checkFile(filename)){
        return FILENAME_EXISTS;
    }

    return "";
}

bool Enter::checkFile( std::string& name) {
    bool file_record;
    int number_end_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        file_record = false;
        number_not_free_blocks = 0;
        number_end_blocks = 0;
        for (int i=0; i<FilesystemSegment::FILE_RECORDS_COUNT; i++) {
            auto &file =  filesystem.filesystemSegment[j].fileRecord[i];
            if(file.recordType != RECORDS_END)
                number_end_blocks += static_cast<int>(file.blockCount);
            if(file.recordType == RECORDS_END || i+1==FilesystemSegment::FILE_RECORDS_COUNT) {
                if ((UtilsFunctions::getSegmentSizeInBlocks(filesystem,j) - number_not_free_blocks) > max_length_file)
                    max_length_file = ( UtilsFunctions::getSegmentSizeInBlocks(filesystem,j) - number_not_free_blocks);
                if (filesystem.filesystemSegment[j].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType !=RECORDS_END) {
                    file_record = true;
                    break;
                }
            }

            if (file.recordType == RECORDS_END || file.recordType == FREE) {
                if( max_length_file < file.blockCount)
                    max_length_file = file.blockCount;

                if(file.blockCount == length) {
                    have_such_number_of_blocks = true;
                }
            }else {
                number_not_free_blocks += file.blockCount;
                if (file.fileName == name) {
                    return true;
                }
            }
        }
        if((UtilsFunctions::getSegmentSizeInBlocks(filesystem,j) - number_end_blocks) == length && !file_record) {
            have_such_number_of_blocks = true;
    }
    }
    return false;
}
std::string Enter::findPlaceForFile() {
    bool insert;
    bool file_record;
    bool if_next_segment  =false;
    int delta_length = 0;
    FileRecord buf;
    int number_end_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++){
        file_record = false;
        number_end_blocks = 0;
        for (int i=0; i<FilesystemSegment::FILE_RECORDS_COUNT; i++) {
            auto &file = filesystem.filesystemSegment[j].fileRecord[i];
            if(insert){
                if(delta_length!=0){
                    buf = file;
                    file.recordType = FREE;
                    file.blockCount = static_cast<uint16_t>(delta_length);
                    strcpy(file.fileName, "12345.123");
                    delta_length = 0;
                }
                else {
                    FileRecord buf1 = file;
                    file = buf;
                    buf = buf1;
                }
            }
            if(file.recordType != RECORDS_END && i+1!=FilesystemSegment::FILE_RECORDS_COUNT)
                number_end_blocks += file.blockCount;
            else {
                if (filesystem.filesystemSegment[j].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType !=
                    RECORDS_END) {
                    file_record = true;
                    break;
                }
            }
            if(file.recordType == RECORDS_END || file.recordType == FREE ) {
                if(!insert && ((file.recordType == RECORDS_END && ((UtilsFunctions::getSegmentSizeInBlocks(filesystem,j) - number_end_blocks) > length) && !have_such_number_of_blocks)
                                   || (file.recordType == RECORDS_END && ((UtilsFunctions::getSegmentSizeInBlocks(filesystem,j) - number_end_blocks) == length) && have_such_number_of_blocks) || ((file.blockCount == length && have_such_number_of_blocks) || (file.blockCount > length && !have_such_number_of_blocks)))){
                    if(file.recordType == FREE)
                        if (filesystem.filesystemSegment[j].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType == RECORDS_END) {
                            delta_length = file.blockCount - length;
                        }
                    RecordType type = file.recordType;
                        file.recordType = REGULAR_FILE;
                        file.blockCount = static_cast<uint16_t>(length);
                    strcpy(file.fileName, filename.c_str());
                    insert = true;
                    if( type == RECORDS_END || have_such_number_of_blocks){
                        if_next_segment= true;
                        break;
                    }
                }

            }
        }
        if(if_next_segment) break;
    }
    if(file_record) return NOT_ENOUGH_FILE_RECORD;
    if(!insert) return NO_FILE_RECORD;

    return "";
}
std::string Enter::run() {
    std::stringstream stream;
    if(!filesystem.isInit) {
        return "File system file not found. Please run init command.";
    }
        if (max_length_file >= length) {
            std::string errorMessage;
            if (errorMessage = findPlaceForFile(); !errorMessage.empty()) return COMMON_ERROR_MESSAGE + errorMessage;
        } else {
            if (length > (UtilsFunctions::getSegmentSizeInBlocks(filesystem,filesystem.filesystemInfo.segmentsCount))) {
                stream << COMMON_ERROR_MESSAGE << TOO_BIG_FILE;
                return stream.str();
            } else {
                stream << COMMON_ERROR_MESSAGE << NO_SPACE;
                return stream.str();
            }
        }
        filesystem.serializer.save(filesystem);
        stream << "File created successfully.";
    return stream.str();
}

std::string Enter::processQuery(Parser &parser) {
    auto poss = parser.getBoolArgs();
    std::string resultMessage;

    if(std::find(std::begin(poss), std::end(poss), "help") != std::end(poss)){
        return help();
    } else if(resultMessage = checkAndAssemble(parser); resultMessage.empty()) { // parser becomes invalid here, if no error
        resultMessage = run();
    } else{
        resultMessage = COMMON_ERROR_MESSAGE + resultMessage;
    }

    return resultMessage;
}

std::string Enter::help() {
    return "usage: enter -l <length_file> \"filename\"";
}

std::string Enter::description() {
    return "creates new file";
}
