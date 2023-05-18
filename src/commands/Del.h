#ifndef MAIN_DEL_H
#define MAIN_DEL_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Del : public BaseCommand {
private:
    Filesystem &filesystem;
    std::string filename;
    RecordType recordtype;

    static constexpr char COMMON_ERROR_MESSAGE[] = "Error in creating file: ";

    static constexpr char NOT_ENOUGH_ARGS[] = "Not enough arguments (see ENTER --help).";
    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see ENTER --help).";
    static constexpr char NO_FILENAME_VALUE[] = "Filename can’t be empty.";
    static constexpr char FILENAME_INCORRECT[] = "Filename has incorrect format.";


    std::string checkAmount(const Parser &parser);

    std::string setFilename(posArgs_t &poss);
protected:
    std::string help() override;
public:
//    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }
    explicit Del(Filesystem *filesystem) : filesystem(*filesystem), filename(), recordtype(){ }
    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    bool checkFile( std::string& name);
    FileRecord& findFile(std::string& name) const;
    std::string run() final;

    std::string processQuery(Parser &parser) final;
};

#endif //MAIN_DEL_H
