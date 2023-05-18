#ifndef MAIN_EMPTY_H
#define MAIN_EMPTY_H

#include "../commands/BaseCommand.h"

class Empty : public BaseCommand {
public:
    Empty();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};

#endif //MAIN_EMPTY_H
