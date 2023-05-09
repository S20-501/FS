#ifndef MAIN_MOVE_H
#define MAIN_MOVE_H

#include "../commands/BaseCommand.h"

class Move : public BaseCommand {
private:
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
    Move();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_MOVE_H
