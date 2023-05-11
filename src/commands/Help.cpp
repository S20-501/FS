#include <sstream>

#include "Help.h"

Help::Help() = default;

std::string Help::getQuery(){
    return "help";
}

std::string Help::checkAndAssemble(Parser &parser) {
    (void) parser; // unused
    return ""; // always correct
}

std::string Help::run() {
    return helpMessage;
}

std::string Help::help() {
    return helpMessage;
}
