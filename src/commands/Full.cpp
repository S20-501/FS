#include <sstream>

#include "Full.h"
#include "utils/utilFunctions.h"

Full::Full() = default;

std::string Full::getQuery(){
    return "full";
}

std::string Full::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    setEmpty(parser.getBoolArgs());
    setHeader(parser.getBoolArgs());
    setHeaderonly(parser.getBoolArgs());

    return errorMessage;
}

std::string Full::checkAmount(const Parser &parser) {
    if(parser.getBoolArgs().size() > 3){
        return WRONGBOOLSAMOUNT;
    }

    return "";
}

void Full::setEmpty(const boolArgs_t &bools) {
    findAndSetBoolArg(bools, empty, "empty", "e");
}

void Full::setHeader(const boolArgs_t &bools) {
    findAndSetBoolArg(bools, header, "header", "h");
}

void Full::setHeaderonly(const boolArgs_t &bools) {
    findAndSetBoolArg(bools, headeronly, "headeronly", "o");
}

std::string Full::run() {
    // return fs_full(empty, header, headeronly);
    std::stringstream stream;
    stream << "full command executed, empty: \"" << empty <<
            "\", header: \"" << header << "\", headeronly: \"" << headeronly << "\"";
    return stream.str();
}

std::string Full::help() {
    return "full help";
}

std::string Full::description() {
    return "lists all files";
}