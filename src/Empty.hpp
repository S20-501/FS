//
// Created by ilyap on 04.05.2023.
//

#ifndef FS_EMPTY_HPP
#define FS_EMPTY_HPP

#include <iostream>
#include <vector>
#include "FileRecord.hpp"
#include "FilesystemInfo.hpp"

class Empty{
private:
    uint16_t fileRecordsCount;
    uint16_t tomSize;
    char* tomLabel;
    std::vector<std::string> flags;
    FileRecord* fileRecords;

public:
    Empty(FileRecord* fileRecords, uint16_t fileRecordsCount, char* tomLabel, uint16_t tomSize, std::vector<std::string> flags) {
        this->fileRecords = fileRecords;
        this->fileRecordsCount = fileRecordsCount;
        this->tomLabel = tomLabel;
        this->tomSize = tomSize;
        this->flags = flags;
    }
    void execute() {
        bool shouldPrintFree = ( std::find(flags.begin(), flags.end(), "empty") != flags.end() ) ||
                               ( std::find(flags.begin(), flags.end(), "e") != flags.end() );

    }
};

#endif //FS_EMPTY_HPP
