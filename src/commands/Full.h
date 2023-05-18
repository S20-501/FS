#ifndef MAIN_FULL_H
#define MAIN_FULL_H

#include "../commands/BaseCommand.h"

class Full : public BaseCommand {
private:
    bool empty;
    bool header;
    bool headeronly;

    static constexpr char WRONGBOOLSAMOUNT[] = "invalid boolean values amount";

    std::string checkAmount(const Parser &parser);

    void setEmpty(const boolArgs_t &bools);
    void setHeader(const boolArgs_t &bools);
    void setHeaderonly(const boolArgs_t &bools);
public:
    Full();

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;
    std::string help() override;
    static std::string description();
};


#endif //MAIN_FULL_H
