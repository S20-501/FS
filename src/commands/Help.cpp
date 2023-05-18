#include <sstream>

#include "Help.h"
#include "UtilsFunctions.hpp"

std::string Help::getQuery(){
    return "help";
}

std::string Help::checkAndAssemble(Parser &parser) {
    (void) parser; // unused
    return ""; // always correct
}

std::string Help::run() {
    std::stringstream str;

    str << "List of all supported commands: " << std::endl;
    str << UtilsFunctions::forEachHelp(&filesystem);

    std::string string = str.str();

    return UtilsFunctions::removeClosingEndl(string);
}

std::string Help::help() {
    return helpMessage;
}

std::string Help::description() {
    return "prints this message";
}