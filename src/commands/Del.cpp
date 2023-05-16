#include <sstream>
#include <cstring>
#include <iostream>

#include "Del.h"
#include "../utils/utilFunctions.h"

#include "../UtilsFunctions.hpp"

std::string Del::getQuery(){
    return "del";
}

std::string Del::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setFilename(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Del::checkAmount(const Parser &parser) {
    if(parser.getPosArgs().size() != 1){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Del::setFilename(posArgs_t &poss) {
    filename = std::move(poss.back());
    poss.pop_back();

    if(filename.empty()){
        return INCORRECTFILENAME;
    }

    return "";
}

std::string Del::run() {
    bool ident= false;
    char name[10] = "free";
    for(int j =0;j<filesystem.filesystemInfo.segmentsCount; j++) {
        for (int i = 0; i < 63; i++) {
            if (filesystem.filesystemSegment[j].fileRecord[i].recordType != REGULAR_PROTECTED && filesystem.filesystemSegment[j].fileRecord[i].recordType != FREE
            && filesystem.filesystemSegment[j].fileRecord[i].recordType != RECORDS_END) {
                if (filename == filesystem.filesystemSegment[j].fileRecord[i].fileName) {
                    strcpy(filesystem.filesystemSegment[j].fileRecord[i].fileName, name);
                    if (i != 62) {
                        if (filesystem.filesystemSegment[j].fileRecord[i + 1].recordType == RECORDS_END) {
                            filesystem.filesystemSegment[j].fileRecord[i].recordType = static_cast<RecordType>(RECORDS_END);
                        } else {
                            filesystem.filesystemSegment[j].fileRecord[i].recordType = static_cast<RecordType>(FREE);
                        }
                    }
                    ident = true;
                    break;
                }
            }
            if (ident) {
                break;
            }
        }
    }// return fs_init(blocks, segments, label);
    filesystem.serializer.save(filesystem);
    std::stringstream stream;
    if(ident) {
        stream << "del command executed, file name: \"" << filename << "\"";
        return stream.str();
    }else
        return INCORRECTFILENAME;

}

std::string Del::help() {
    return "del help";
}
