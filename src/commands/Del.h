#ifndef MAIN_DEL_H
#define MAIN_DEL_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Del : public BaseCommand {
private:
    Filesystem &filesystem;
    std::string filename;
    RecordType recordtype;

    static constexpr char COMMON_ERROR_MESSAGE[] = "Error in delete file: ";

    static constexpr char NOT_ENOUGH_ARGS[] = "Not enough arguments (see DEL --help).";
    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see DEL --help).";
    static constexpr char NO_FILENAME_VALUE[] = "Filename can’t be empty.";


    std::string checkAmount(const Parser &parser);

    std::string setFilename(posArgs_t &poss);
public:
//    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }
    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(), recordtype(){ }
    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    bool checkFile( std::string& name);
    FileRecord& findFile(std::string& name) const;
    std::string run() final;

    std::string help() override;
    static std::string description();

    std::string processQuery(Parser &parser) final;
};

#endif //MAIN_DEL_H
