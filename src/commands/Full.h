#ifndef MAIN_FULL_H
#define MAIN_FULL_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Full : public BaseCommand {
private:
    bool empty = false;
    bool header = false;
    bool headeronly = false;

    Filesystem &filesystem;

    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see FULL --help).";

    std::string checkAmount(const Parser &parser);

    void setEmpty(const boolArgs_t &bools);
    void setHeader(const boolArgs_t &bools);
    void setHeaderonly(const boolArgs_t &bools);
public:
    explicit Full(Filesystem *filesystem): filesystem(*filesystem) {}

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};


#endif //MAIN_FULL_H
