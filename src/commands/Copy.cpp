#include <sstream>

#include "Copy.h"
//#include "utilFunctions.h"

Copy::Copy() = default;

std::string Copy::getQuery(){
    return "copy";
}

std::string Copy::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setNewFile(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setOldFile(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Copy::checkAmount(const Parser &parser) {
    if(parser.getPosArgs().size() != 2){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Copy::setOldFile(posArgs_t &poss) {
    oldFile = std::move(poss.back());
    poss.pop_back();

    if(oldFile.empty()){
        return INCORRECTOLDFILE;
    }

    return "";
}

std::string Copy::setNewFile(posArgs_t &poss) {
    newFile = std::move(poss.back());
    poss.pop_back();

    if(newFile.empty()){
        return INCORRECTNEWFILE;
    }

    return "";
}

std::string Copy::run() {
    // return fs_init(blocks, segments, label);
    std::stringstream stream;
    stream << "copy command executed, old file: \"" << oldFile <<
            "\", new file: \"" << newFile << "\"";
    return stream.str();
}

std::string Copy::help() {
    return "copy help";
}

std::string Copy::description() {
    return "copy help";
}