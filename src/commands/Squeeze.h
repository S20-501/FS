#ifndef MAIN_SQUEEZE_H
#define MAIN_SQUEEZE_H

#include "../commands/BaseCommand.h"

class Squeeze : public BaseCommand {
public:
    Squeeze();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};

#endif //MAIN_SQUEEZE_H
