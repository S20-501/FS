#ifndef MAIN_MOVE_H
#define MAIN_MOVE_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Move : public BaseCommand {
private:
    Filesystem &filesystem;
    std::string oldFile;
    std::string newFile;
    int freespace = 0;
    int inSegment = 0;
    RecordType recordtype;

    static constexpr char COMMON_ERROR_MESSAGE[] = "Error in move file: ";

    static constexpr char NOT_ENOUGH_ARGS[] = "Not enough arguments (see MOVE --help).";
    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see MOVE --help).";
    static constexpr char NO_OLD_FILENAME_VALUE[] = "Old filename can’t be empty.";
    static constexpr char NO_NEW_FILENAME_VALUE[] = "Old filename filename can’t be empty.";
    static constexpr char TARGET_INSUFFICENT[] = "The size of the target file is insufficient to store the source.";

    std::string checkAmount(const Parser &parser);

    std::string setOldFile(posArgs_t &poss);
    std::string setNewFile(posArgs_t &poss);
public:
    explicit Move(Filesystem *filesystem) : filesystem(*filesystem), oldFile(), newFile(), recordtype(){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    std::string run() final;

    std::string help() override;
    static std::string description();

    FileRecord & findFile(std::string& name);
    bool checkFile( std::string& name);

    std::string processQuery(Parser &parser) final;
};

#endif //MAIN_MOVE_H
