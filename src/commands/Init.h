#ifndef MAIN_INIT_H
#define MAIN_INIT_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Init : public BaseCommand {
private:
    Filesystem &filesystem;
    uint16_t blocks = 0;
    uint16_t segments = 0;
    std::string label;

    static constexpr char DEFAULT_LABEL[] = "DEFAULT";

    static constexpr char COMMON_ERROR_MESSAGE[] = "Error in creating filesystem: ";

    static constexpr char NOT_ENOUGH_ARGS[] = "Not enough arguments (see INIT --help).";
    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see INIT --help).";
    static constexpr char BLOCKS_CANT_CONVERT[] = "Blocks has incorrect format.";
    static constexpr char BLOCKS_RESTRICTED[] = "Blocks must be in range [1, 65535].";
    static constexpr char NO_BLOCKS_VALUE[] = "Blocks can’t be empty.";
    static constexpr char SEGMENTS_CANT_CONVERT[] = "Segments has incorrect format.";
    static constexpr char SEGMENTS_RESTRICTED[] = "Segments must be in range [1, 31].";
    static constexpr char NO_SEGMENTS_VALUE[] = "Segments can’t be empty.";
    static constexpr char LABEL_TOO_LONG[] = "The label is too long (maximum 10 chars).";
    static constexpr char LABEL_INCORRECT[] = "Label has incorrect format.";

    static std::string checkAmount(const Parser &parser);

    std::string setBlocks(const keyArgs_t &keys);
    std::string setSegments(const keyArgs_t &keys);
    std::string setLabel(posArgs_t &poss);
protected:
    std::string help() override;
public:
    explicit Init(Filesystem *filesystem) : filesystem(*filesystem), label(){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;

    std::string processQuery(Parser &parser) final;
};


#endif //MAIN_INIT_H
