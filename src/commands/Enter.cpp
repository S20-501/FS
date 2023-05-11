#include <sstream>

#include "Enter.h"
#include "utils/utilFunctions.h"

Enter::Enter() = default;

std::string Enter::getQuery(){
    return "enter";
}

std::string Enter::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setLength(parser.getKeyArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setFilename(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Enter::checkAmount(const Parser &parser) {
    if(parser.getKeyArgs().size() != 1){
        return WRONGKEYSAMOUNT;
    }
    if(parser.getPosArgs().size() != 1){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Enter::setLength(const keyArgs_t &keys) {
    if(auto it = keys.find("length"); it != keys.end() || ((it = keys.find("l")) != keys.end())) {
        // convert to int
        if(convertToNumber(it->second, length)) return LENGTHCANTCONVERT;

        // check restrictions
        if(length < 1 || 65535 < length){
            return LENGTHRESTRICTED;
        }
    } else {
        return NOLENGTHVALUE;
    }

    return "";
}

std::string Enter::setFilename(posArgs_t &poss) {
    filename = std::move(poss.back());
    poss.pop_back();

    if(filename.size() > 10 || !isASCII(filename)){
        return INCORRECTFILENAME;
    }

    return "";
}

std::string Enter::run() {
    // return fs_init(blocks, segments, label);
    std::stringstream stream;
    stream << "enter command executed, length: \"" << length <<
            "\", filename: \"" << filename << "\"";
    return stream.str();
}

std::string Enter::help() {
    return "enter help";
}
