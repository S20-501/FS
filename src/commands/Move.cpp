#include <sstream>
#include <iostream>
#include "Move.h"
#include "../utils/utilFunctions.h"
#include "../exceptions/FileCannotCreate.hpp"
#include "../UtilsFunctions.hpp"

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
    if(parser.getPosArgs().size() != 2){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Move::setOldFile(posArgs_t &poss) {
    oldFile = std::move(poss.back());
    poss.pop_back();

    if(oldFile.empty() || !checkFile(oldFile)){
        return INCORRECTOLDFILE;
    }
    return "";
}

std::string Move::setNewFile(posArgs_t &poss) {
    newFile = std::move(poss.back());
    poss.pop_back();
    if(newFile.empty() ||  !checkFile(newFile)){
        return INCORRECTNEWFILE;
    }

    return "";
}
bool Move::checkFile( std::string& name) const {
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++) {
        for (auto & i : filesystem.filesystemSegment[j].fileRecord) {
            if (i.recordType != RECORDS_END ||
                i.recordType != FREE) {
                if (i.fileName == name) {
                    return true;
                }
            }
        }
    }
    return false;
}
FileRecord& Move::findFile(std::string& name) const {
    for (int j = 0; j < filesystem.filesystemInfo.segmentsCount; j++)
        for (auto &i: filesystem.filesystemSegment[j].fileRecord) {
            if(i.fileName == name)
                return i;
        }
}
std::string Move::run() {
    // return fs_init(blocks, segments, label);


    FileRecord& oldfile = findFile(oldFile);
    FileRecord& newfile = findFile(newFile);
    FileRecord old = findFile(oldFile);
    oldfile = newfile;
    newfile = old;
    std::stringstream stream;
    filesystem.serializer.save(filesystem);
    stream << "move command executed, old file: \"" << oldFile <<
            "\", new file: \"" << newFile << "\"";
    return stream.str();
}

std::string Move::help() {
    return R"(usage: del "old filename" "new filename")";
}
