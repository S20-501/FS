#include <sstream>

#include "Help.h"
#include "FSCommands.hpp"
#include "UtilsFunctions.hpp"

std::string Help::getQuery(){
    return "help";
}

std::string Help::checkAndAssemble(Parser &parser) {
    (void) parser; // unused
    return ""; // always correct
}

template<std::size_t index, class ...Args>
static typename std::enable_if<index == std::tuple_size_v<FSCommandsTuple>, std::string>::type
forEachHelp([[maybe_unused]] Args... args){
    return "";
}

template<std::size_t index = 0, class ...Args>
static typename std::enable_if<index < std::tuple_size_v<FSCommandsTuple>, std::string>::type
forEachHelp(Args... args){
    using CommandClass = std::tuple_element_t<index, FSCommandsTuple>;

    std::stringstream str;

    auto command = std::make_shared<CommandClass>(args...);

    str << "    " << CommandClass::getQuery() << " - " << CommandClass::description() << std::endl
        << "        " << command->help() << std::endl
        << forEachHelp<index + 1>(args...);

    return str.str();
}

std::string Help::run() {
    std::stringstream str;

    str << "List of all supported commands: " << std::endl;
    str << forEachHelp(&filesystem);

    std::string string = str.str();

    return UtilsFunctions::removeClosingEndl(string);
}

std::string Help::help() {
    return helpMessage;
}

std::string Help::description() {
    return "prints this message";
}