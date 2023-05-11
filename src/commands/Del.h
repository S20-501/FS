#ifndef MAIN_DEL_H
#define MAIN_DEL_H

#include "../commands/BaseCommand.h"

class Del : public BaseCommand {
private:
    std::string filename;

    static constexpr char WRONGPOSSAMOUNT[] = "invalid positional values amount";
    static constexpr char INCORRECTFILENAME[] = "file name value is incorrect";

    std::string checkAmount(const Parser &parser);

    std::string setFilename(posArgs_t &poss);
protected:
    std::string help() override;
public:
    Del();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
};

#endif //MAIN_DEL_H
