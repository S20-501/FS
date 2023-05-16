#ifndef MAIN_EMPTY_H
#define MAIN_EMPTY_H

#include "BaseCommand.h"
#include "../Filesystem.hpp"

class Empty : public BaseCommand {
private:
    bool empty = false;

    Filesystem &filesystem;
    static constexpr char WRONGBOOLSAMOUNT[] = "invalid boolean values amount";
    std::string checkAmount(const Parser &parser);
    void setEmpty(const boolArgs_t &bools);
protected:
    std::string help() override;
public:
    explicit Empty(Filesystem *filesystem) : filesystem(*filesystem) {}

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_EMPTY_H
