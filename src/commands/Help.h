#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Help : public BaseCommand {
private:
    Filesystem &filesystem;
    static constexpr char helpMessage[] = "usage: help";
public:
    explicit Help(Filesystem *filesystem) : filesystem(*filesystem){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};

#endif //MAIN_HELP_H
