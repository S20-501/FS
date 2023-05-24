#ifndef MAIN_ENTER_H
#define MAIN_ENTER_H

#include "BaseCommand.h"
#include "Filesystem.hpp"

class Enter : public BaseCommand {
private:
    Filesystem &filesystem;
    int length = 0;
    std::string filename;
    bool have_such_number_of_blocks = false;
    int number_not_free_blocks=0;
    int max_length_file= 0;

    static constexpr char COMMON_ERROR_MESSAGE[] = "Error in creating file: ";

    static constexpr char NOT_ENOUGH_ARGS[] = "Not enough arguments (see ENTER --help).";
    static constexpr char TOO_MANY_ARGS[] = "Too many arguments (see ENTER --help).";
    static constexpr char LENGTH_CANT_CONVERT[] = "Length has incorrect format.";
    static constexpr char LENGTH_RESTRICTED[] = "Length must be in range [1, 65535].";
    static constexpr char NO_LENGTH_VALUE[] = "Length can’t be empty.";
    static constexpr char FILENAME_TOO_LONG[] = "The filename is too long (maximum 10 chars).";
    static constexpr char FILENAME_INCORRECT[] = "Filename has incorrect format.";
    static constexpr char FILENAME_EXISTS[] = "The file with that name already exists on disk.";
    static constexpr char NO_FILENAME_VALUE[] = "Filename can’t be empty.";
    static constexpr char TOO_BIG_FILE[] = "The length of this file is too big for this filesystem.";
    static constexpr char NO_FILE_RECORD[] = "It is not possible to create a record on disk. Perform disk SQUEEZE.";
    static constexpr char NO_SPACE[] = "There is no such free space on disk. The available storage space has been exceeded.";
    static constexpr char NOT_ENOUGH_FILE_RECORD[] = "Not enough filerecord.";

    std::string checkAmount(const Parser &parser);

    std::string setLength(const keyArgs_t &keys);
    std::string setFilename(posArgs_t &poss);
public:
    explicit Enter(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    bool checkFile( std::string& name);
    std::string findPlaceForFile();
    std::string run() final;

    std::string help() override;
    static std::string description();

    std::string processQuery(Parser &parser) final;
};

#endif //MAIN_ENTER_H
