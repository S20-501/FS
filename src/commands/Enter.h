#ifndef MAIN_ENTER_H
#define MAIN_ENTER_H

#include "BaseCommand.h"
#include "../Filesystem.hpp"

class Enter : public BaseCommand {
private:
    Filesystem &filesystem;
    int length = 0;
    std::string filename;
    bool have_such_number_of_bytes = false;
    int max_length_file= 0;
    static constexpr char NOSPACE[] = "there is no space, do defragmentation";
    static constexpr char WRONGKEYSAMOUNT[] = "invalid key values amount";
    static constexpr char WRONGPOSSAMOUNT[] = "invalid positional values amount";
    static constexpr char LENGTHCANTCONVERT[] = "length incorrect value";
    static constexpr char LENGTHRESTRICTED[] = "length incorrect value";
    static constexpr char NOLENGTHVALUE[] = "no length key value";
    static constexpr char INCORRECTFILENAME[] = "label value is incorrect";

    std::string checkAmount(const Parser &parser);

    std::string setLength(const keyArgs_t &keys);
    std::string setFilename(posArgs_t &poss);
protected:
    std::string help() override;
public:
    explicit Enter(Filesystem *filesystem) : filesystem(*filesystem), filename(){ }

    static std::string getQuery();

    std::string checkAndAssemble(Parser &parser) final;
    bool checkFile( std::string& name);
    std::string findPlaceForFile();
    std::string run() final;
};

#endif //MAIN_ENTER_H
