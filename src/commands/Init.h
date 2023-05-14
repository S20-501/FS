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

    static constexpr char DEFAULTLABEL[] = "DEFAULT";

    static constexpr char WRONGKEYSAMOUNT[] = "invalid key values amount";
    static constexpr char WRONGPOSSAMOUNT[] = "invalid positional values amount";
    static constexpr char BLOCKSCANTCONVERT[] = "blocks incorrect value";
    static constexpr char BLOCKSRESTRICTED[] = "blocks incorrect value";
    static constexpr char NOBLOCKSVALUE[] = "no blocks key value";
    static constexpr char SEGMENTSCANTCONVERT[] = "segments incorrect value";
    static constexpr char SEGMENTSRESTRICTED[] = "segments incorrect value";
    static constexpr char NOSEGMENTSVALUE[] = "no segments key value";
    static constexpr char LABELINCORRECT[] = "label value is incorrect";

    std::string checkAmount(const Parser &parser);

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
};


#endif //MAIN_INIT_H
