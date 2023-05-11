#include <sstream>

#include "Move.h"
#include "utils/utilFunctions.h"

Move::Move() = default;

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

    if(oldFile.empty()){
        return INCORRECTOLDFILE;
    }

    return "";
}

std::string Move::setNewFile(posArgs_t &poss) {
    newFile = std::move(poss.back());
    poss.pop_back();

    if(newFile.empty()){
        return INCORRECTNEWFILE;
    }

    return "";
}

std::string Move::run() {
    // return fs_init(blocks, segments, label);
    std::stringstream stream;
    stream << "move command executed, old file: \"" << oldFile <<
            "\", new file: \"" << newFile << "\"";
    return stream.str();
}

std::string Move::help() {
    return "move help";
}
