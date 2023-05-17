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

    if(filename.empty() || !checkFile(filename)){
        return INCORRECTFILENAME;
    }

    return "";
}
bool Del::checkFile( std::string& name) {
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        for (int i=0; i<FilesystemSegment::FILE_RECORDS_COUNT; i++) {
            auto & file = filesystem.filesystemSegment[j].fileRecord[i];
            if (file.recordType != RECORDS_END ||
                file.recordType != FREE) {
                if (file.fileName == name) {
                    if(filesystem.filesystemSegment[j].fileRecord[i+1].recordType == RECORDS_END) {
                        recordtype = RECORDS_END;
                    }
                    else
                        recordtype = FREE;
                    return true;
                }
            }
        }
    }
    return false;
}
FileRecord& Del::findFile(std::string& name) const {
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++)
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if(i.fileName == name)
                return i;
        }
}

std::string Del::run() {
    FileRecord &del = findFile(filename);
    del.recordType = recordtype;
    strcpy(del.fileName,"12345.123");
    filesystem.serializer.save(filesystem);
    std::stringstream stream;
    stream << "del command executed, file name: \"" << filename << "\"";
    return stream.str();
}

std::string Del::help() {
    return R"(usage: del "filename")";
}
