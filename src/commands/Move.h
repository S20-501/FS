#ifndef MAIN_MOVE_H
#define MAIN_MOVE_H

#include "BaseCommand.h"
#include "../Filesystem.hpp"

class Move : public BaseCommand {
private:
    Filesystem &filesystem;
    std::string oldFile;
    std::string newFile;

    static constexpr char WRONGPOSSAMOUNT[] = "invalid positional values amount";
    static constexpr char INCORRECTOLDFILE[] = "old file name value is incorrect";
    static constexpr char INCORRECTNEWFILE[] = "new file name value is incorrect";

    std::string checkAmount(const Parser &parser);

    std::string setOldFile(posArgs_t &poss);
    std::string setNewFile(posArgs_t &poss);
protected:
    std::string help() override;
public:
    explicit Move(Filesystem *filesystem) : filesystem(*filesystem), oldFile(), newFile(){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    FileRecord& findFile(std::string& name) const ;
    bool checkFile( std::string& name) const;
};

#endif //MAIN_MOVE_H
