#include <sstream>
#include <iostream>
#include <cstring>
#include "Move.h"
#include "exceptions/FileCannotCreate.hpp"
#include "exceptions/FileNotFoundException.hpp"

std::string Move::getQuery(){
    return "move";
}

std::string Move::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setNewFile(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setOldFile(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Move::checkAmount(const Parser &parser) {
    if(parser.getPosArgs().size() < 2){
        return TOO_MANY_ARGS;
    }

    if(parser.getPosArgs().size() > 2){
        return NOT_ENOUGH_ARGS;
    }
    return "";
}

std::string Move::setOldFile(posArgs_t &poss) {
    oldFile = std::move(poss.back());
    poss.pop_back();

    if(oldFile.empty()){
        return NO_OLD_FILENAME_VALUE;
    }

    if(!checkFile(oldFile)){
        return OLD_FILENAME_EXISTS;
    }
    return "";
}

std::string Move::setNewFile(posArgs_t &poss) {
    newFile = std::move(poss.back());
    poss.pop_back();
    if(newFile.empty()){
        return NO_NEW_FILENAME_VALUE;
    }

    if(!checkFile(newFile)){
        return NEW_FILENAME_EXISTS;
    }

    return "";
}
bool Move::checkFile( std::string& name) {
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
FileRecord & Move::findFile(std::string& name) {
    int busy_space;
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        busy_space = 0;
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if (i.recordType != RECORDS_END)
                busy_space += static_cast<int>(i.blockCount);
            else {
                freespace = filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - busy_space;
                inSegment = j;
            }
                if (i.fileName == name)
                return i;
        }
    }
    throw FileNotFoundException();
}

std::string Move::run() {
    // return fs_init(blocks, segments, label);
    bool have_free_memory = false;
    FileRecord& oldfile = findFile(oldFile);
    int oldInSegment = inSegment;
    FileRecord& newfile = findFile(newFile);
    int freeSpaceNewSegment = freespace;
    FileRecord old = oldfile;
    if(filesystem.filesystemSegment[oldInSegment].fileRecord[FilesystemSegment::FILE_RECORDS_COUNT - 1].recordType == RECORDS_END) {
        if (oldfile.blockCount >= newfile.blockCount) {
            if (freeSpaceNewSegment >= oldfile.blockCount - newfile.blockCount)
                have_free_memory = true;
        } else {
            have_free_memory = true;
        }
    }
    if(have_free_memory) {
        oldfile.recordType = recordtype;
        if(recordtype == RECORDS_END)
            oldfile.blockCount = 0;
        strcpy(oldfile.fileName,"12345.123");
        newfile = old;
    } else
        return TARGET_INSUFFICENT;
    std::stringstream stream;
    filesystem.serializer.save(filesystem);
    stream << "File moved successfully.";
    return stream.str();
}

std::string Move::processQuery(Parser &parser) {
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

std::string Move::help() {
    return R"(usage: move "old filename" "new filename")";
}
