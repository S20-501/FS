#ifndef MAIN_SQUEEZE_H
#define MAIN_SQUEEZE_H

#include "BaseCommand.h"
#include "../Filesystem.hpp"

class Squeeze : public BaseCommand {
private:
    Filesystem& filesystem;
protected:
    std::string help() override;
public:
    explicit Squeeze(Filesystem* filesystem) : filesystem(*filesystem) {}

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_SQUEEZE_H
