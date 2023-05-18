#include <sstream>
#include <cstring>
#include <iostream>

#include "Del.h"

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
    if(parser.getPosArgs().size() > 1){
        return TOO_MANY_ARGS;
    }

    if(parser.getPosArgs().size() < 1){
        return NOT_ENOUGH_ARGS;
    }

    return "";
}

std::string Del::setFilename(posArgs_t &poss) {
    filename = std::move(poss.back());
    poss.pop_back();

    if(filename.empty()){
        return NO_FILENAME_VALUE;
    }

    if(!checkFile(filename)){
        return FILENAME_INCORRECT;
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
    throw std::runtime_error("File not found");
}

std::string Del::run() {
    FileRecord &del = findFile(filename);
    del.recordType = recordtype;
    if(recordtype == RECORDS_END)
        del.blockCount = 0;
    strcpy(del.fileName,"12345.123");
    filesystem.serializer.save(filesystem);
    std::stringstream stream;
    stream << "del command executed, file name: \"" << filename << "\"";
    return stream.str();
}

std::string Del::processQuery(Parser &parser) {
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

std::string Del::help() {
    return R"(usage: del "filename")";
}

std::string Del::description() {
    return "deletes file";
}