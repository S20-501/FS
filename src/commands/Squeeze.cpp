#include <sstream>

#include "Squeeze.h"

Squeeze::Squeeze() = default;

std::string Squeeze::getQuery(){
    return "squeeze";
}

std::string Squeeze::checkAndAssemble(Parser &parser) {
    return ""; // always correct
}

std::string Squeeze::run() {
    // return fs_squeeze();
    std::stringstream stream;
    stream << "squeeze command executed";
    return stream.str();
}

std::string Squeeze::help() {
    return "squeeze help";
}

std::string Squeeze::description() {
    return "performs defragmentation";
}