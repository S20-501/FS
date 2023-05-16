#include <sstream>
#include <cstring>
#include <iostream>

#include "Del.h"
#include "../utils/utilFunctions.h"
#include "../exceptions/FileCannotCreate.hpp"
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
            auto file = filesystem.filesystemSegment[j].fileRecord[i];
            if (file.recordType != REGULAR_PROTECTED && file.recordType != FREE && file.recordType != RECORDS_END) {
                std::cerr << file.fileName << std::endl;
                if (filename == file.fileName) {
                    strcpy(file.fileName, name);
                    std::cerr << file.fileName << std::endl;
                    if (i != 62) {
                        if (filesystem.filesystemSegment[j].fileRecord[i + 1].recordType == RECORDS_END) {
                            file.recordType = static_cast<RecordType>(RECORDS_END);
                        } else {
                            file.recordType = static_cast<RecordType>(FREE);
                        }
                    }
                    ident = true;
                    break;
                }
            }
            if (ident)
                break;
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
