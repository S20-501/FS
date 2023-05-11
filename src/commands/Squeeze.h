#ifndef MAIN_SQUEEZE_H
#define MAIN_SQUEEZE_H

#include "../commands/BaseCommand.h"

class Squeeze : public BaseCommand {
protected:
    std::string help() override;
public:
    Squeeze();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_SQUEEZE_H
