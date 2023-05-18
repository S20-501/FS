#ifndef MAIN_EMPTY_H
#define MAIN_EMPTY_H

#include <map>

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Empty : public BaseCommand {
private:
    bool empty = false;

    Filesystem &filesystem;

    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see EMPTY --help).";

    std::string checkAmount(const Parser &parser);
    void setEmpty(const boolArgs_t &bools);

    static void updateAvailableSizeBlocks(std::map<int, int>& availableSizeBlocks, int keyToAdd, int valueToAdd);
public:
    explicit Empty(Filesystem *filesystem) : filesystem(*filesystem) {}

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};

#endif //MAIN_EMPTY_H
