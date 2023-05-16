#ifndef MAIN_DEL_H
#define MAIN_DEL_H

#include "BaseCommand.h"
#include "../Filesystem.hpp"

class Del : public BaseCommand {
private:
    Filesystem &filesystem;
    std::string filename;

    static constexpr char WRONGPOSSAMOUNT[] = "invalid positional values amount";
    static constexpr char INCORRECTFILENAME[] = "file name value is incorrect";

    std::string checkAmount(const Parser &parser);

    std::string setFilename(posArgs_t &poss);
protected:
    std::string help() override;
public:
//    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }
    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }
    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_DEL_H
