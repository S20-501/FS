#include <sstream>

#include "Del.h"
#include "utils/utilFunctions.h"

Del::Del() = default;

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
    // return fs_init(blocks, segments, label);
    std::stringstream stream;
    stream << "del command executed, file name: \"" << filename << "\"";
    return stream.str();
}

std::string Del::help() {
    return "del help";
}

std::string Del::description() {
    return "deletes file";
}