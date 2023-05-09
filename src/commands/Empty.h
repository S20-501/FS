#ifndef MAIN_EMPTY_H
#define MAIN_EMPTY_H

#include "../commands/BaseCommand.h"

class Empty : public BaseCommand {
protected:
    std::string help() override;
public:
    Empty();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_EMPTY_H
