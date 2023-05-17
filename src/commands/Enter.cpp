#include <sstream>
#include <cstring>
#include <iostream>

#include "Enter.h"
#include "../utils/utilFunctions.h"
#include "../exceptions/FileCannotCreate.hpp"
#include "../UtilsFunctions.hpp"

//Enter::Enter()

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
    if(parser.getKeyArgs().size() != 1){
        return WRONGKEYSAMOUNT;
    }
    if(parser.getPosArgs().size() != 1){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Enter::setLength(const keyArgs_t &keys) {
    if(auto it = keys.find("length"); it != keys.end() || ((it = keys.find("l")) != keys.end())) {
        // convert to int
        if(UtilsFunctions::convertToNumber(it->second, length)) return LENGTHCANTCONVERT;

        // check restrictions
        if(length < 1 || 65535 < length){
            return LENGTHRESTRICTED;
        }
    } else {
        return NOLENGTHVALUE;
    }

    return "";
}

std::string Enter::setFilename(posArgs_t &poss) {
    filename = std::move(poss.back());
    poss.pop_back();
    if(filename.size() > 10 || !UtilsFunctions::isASCII(filename) || checkFile(filename)){
        return INCORRECTFILENAME;
    }

    return "";
}

bool Enter::checkFile( std::string& name) {
    int number_end_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        number_not_free_blocks = 0;
        number_end_blocks = 0;
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if(i.recordType != RECORDS_END)
                number_end_blocks += static_cast<int>(i.blockCount);
            if (i.recordType == RECORDS_END || i.recordType == FREE) {
                if(max_length_file < i.blockCount)
                    max_length_file = i.blockCount;
                if(i.recordType == RECORDS_END) {
                    if ((filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks) > max_length_file)
                        max_length_file = ( filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks);
                }
                if(i.blockCount == length) {
                    have_such_number_of_bytes = true;
                }
            }else {
                number_not_free_blocks += i.blockCount;
                if (i.fileName == name) {
                    return true;
                }
            }
        }
        if((filesystem.filesystemInfo.blocksCount/filesystem.filesystemInfo.segmentsCount - number_end_blocks) == length) {
            have_such_number_of_bytes = true;
    }
    }
    return false;
}
std::string Enter::findPlaceForFile() {
    bool insert;
    bool file_record = false;
    bool if_next_segment  =false;
    int delta_length = 0;
    FileRecord buf;
    int number_end_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++){
        number_end_blocks = 0;
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if(insert){
                have_such_number_of_bytes = false;
                if(delta_length!=0){
                    buf = i;
                    i.recordType = FREE;
                    i.blockCount = static_cast<uint16_t>(delta_length);
                    strcpy(i.fileName, "12345.123");
                    delta_length = 0;
                }
                else {
                    FileRecord buf1 = i;
                    i = buf;
                    buf = buf1;
                }
            }
            if(i.recordType != RECORDS_END)
                number_end_blocks += i.blockCount;
            if(i.recordType == RECORDS_END || i.recordType == FREE ) {
                    if(!insert && ((i.recordType == RECORDS_END && ((filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_end_blocks) > length) && !have_such_number_of_bytes)
                        || (i.recordType == RECORDS_END && ((filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_end_blocks) == length) && have_such_number_of_bytes) || ((i.blockCount == length && have_such_number_of_bytes) || (i.blockCount > length && !have_such_number_of_bytes)))){
                    if(i.recordType == FREE) {
                        if (filesystem.filesystemSegment[j].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType == RECORDS_END) {
                            delta_length = i.blockCount - length;
                        } else {
                            if(filesystem.filesystemSegment[filesystem.filesystemInfo.segmentsCount-1].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType != RECORDS_END)
                                file_record  = true;
                            break;
                        }
                    }
                    RecordType type = i.recordType;
                    i.recordType = REGULAR_FILE;
                    i.blockCount = static_cast<uint16_t>(length);
                    strcpy(i.fileName, filename.c_str());
                    insert = true;
                    if( type == RECORDS_END || have_such_number_of_bytes){
                        if_next_segment= true;
                        break;
                    }
                }

            }
        }
        if(if_next_segment || file_record) break;
    }
    if(file_record || !insert) return NOFILERECORD;

    return "";
}
std::string Enter::run() {
 if((filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks) >= length){
        std::string errorMessage;
        if (errorMessage = findPlaceForFile(); !errorMessage.empty()) return errorMessage;
 }else
       return NOSPACE;
//   bool ident= false;
//    for(int j =0;j<filesystem.filesystemInfo.segmentsCount; j++)
//    for(auto & i : filesystem.filesystemSegment[j].fileRecord){
//        if(i.recordType != REGULAR_FILE){
//          const char* name= filename.c_str();
//          strcpy(i.fileName,name);
//          i.blockCount = static_cast<uint16_t>(length);
//          i.recordType = static_cast<RecordType>(REGULAR_FILE);
//          ident = true;
//          break;
//      }
//       if(ident)
//        break;
//    }
    filesystem.serializer.save(filesystem);
    std::stringstream stream;
    stream << "enter command executed, length: \"" << length <<
            "\", filename: \"" << filename << "\"";
    return stream.str();
}

std::string Enter::help() {
    return "usage: enter -l <length_file> \"filename\"";
}

