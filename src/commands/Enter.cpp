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
//    std::cerr<<checkFile(filename)<<std::endl;
    if(filename.size() > 10 || !UtilsFunctions::isASCII(filename) || checkFile(filename)){
        return INCORRECTFILENAME;
    }

    return "";
}

bool Enter::checkFile( std::string& name) {
    int number_not_free_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        number_not_free_blocks = 0;
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if (i.recordType == RECORDS_END || i.recordType == FREE) {
//                if(max_length_file < i.blockCount)
//                max_length_file = i.blockCount;
                if(i.blockCount == length) {
                    have_such_number_of_bytes = true;
                }
            }else {
                number_not_free_blocks += static_cast<int>(i.blockCount);
                if (i.fileName == name) {
                    return true;
                }
            }
        }
        if((filesystem.filesystemInfo.blocksCount/filesystem.filesystemInfo.segmentsCount - number_not_free_blocks) == length) {
//            std::cerr<<max_length_file<<std::endl;
//            std::cerr<<filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks<<std::endl;
            have_such_number_of_bytes = true;
//
//
    }
    }
    return false;
}
std::string Enter::findPlaceForFile() {
    bool insert = false;
    bool file_record = false;
    bool if_next_segment  =false;
    int delta_length = 0;
    FileRecord buf;
    FileRecord buf1;
    int number_not_free_blocks;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++){
        number_not_free_blocks = 0;
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
                else
//                    buf = i;
                    i= buf;
                    buf = i;
            }
            if(i.recordType == RECORDS_END || i.recordType == FREE ) {
                if(max_length_file < i.blockCount)
                    max_length_file = i.blockCount;
//                if(i.blockCount == length) {
//                    have_such_number_of_bytes = true;
//                }
                if(i.recordType == RECORDS_END) {
                    if ((filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks) > max_length_file)
                        max_length_file = ( filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - number_not_free_blocks);
                }
                std::cerr<<max_length_file<<std::endl;
                if(!insert && ((i.recordType == RECORDS_END && max_length_file > length && !have_such_number_of_bytes) || (i.recordType == RECORDS_END && max_length_file == length && have_such_number_of_bytes) || ((i.blockCount == length && have_such_number_of_bytes) || (i.blockCount > length && !have_such_number_of_bytes)))){
                    if(i.recordType == FREE) {
                        if (filesystem.filesystemSegment[j].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType == RECORDS_END) {
                            delta_length = i.blockCount - length;
                        } else {
                            file_record = true;
                            break;
                        }
                    }
//                    if(max_length_file >= length && i.blockCount == RECORDS_END && have_such_number_of_bytes){
//                        if_next_segment= true;
//                        break;
//                    }
//                    RecordType type = i.recordType;
                    i.recordType = REGULAR_FILE;
                    i.blockCount = static_cast<uint16_t>(length);
                    strcpy(i.fileName, filename.c_str());
                    insert = true;
                    if(have_such_number_of_bytes) {
                        break;
                    }
                }

            }
            else {
                number_not_free_blocks += static_cast<int>(i.blockCount);
            }
        }
        if(insert || file_record || if_next_segment) break;
    }
//    if(!file_record) return "no such file record";
    if(!insert) return LENGTHRESTRICTED;

    return "";
}
std::string Enter::run() {
//    if( max_length_file >= length ) {
        std::string errorMessage;
        if (errorMessage = findPlaceForFile(); !errorMessage.empty()) return errorMessage;
//    }else
//        return NOSPACE;
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
    // return fs_init(blocks, segments, label);
    std::stringstream stream;
    stream << "enter command executed, length: \"" << length <<
            "\", filename: \"" << filename << "\"";
    return stream.str();
}

std::string Enter::help() {
    return "usage: enter -l <length_file> \"filename\"";
}

