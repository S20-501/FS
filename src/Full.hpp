//
// Created by ilyap on 04.05.2023.
//

#ifndef FS_FULL_HPP
#define FS_FULL_HPP

#include <iostream>
#include <vector>
#include "FileRecord.hpp"
#include "FilesystemInfo.hpp"

class Full {
private:
    uint16_t fileRecordsCount;
    uint16_t tomSize;
    char* tomLabel;
    std::vector<std::string> flags;
    FileRecord* fileRecords;


public:
    Full(FileRecord* fileRecords, uint16_t fileRecordsCount, char* tomLabel, uint16_t tomSize, std::vector<std::string> flags) {
        this->fileRecords = fileRecords;
        this->fileRecordsCount = fileRecordsCount;
        this->tomLabel = tomLabel;
        this->tomSize = tomSize;
        this->flags = flags;
    }

    void execute() {
            bool shouldPrintFree = ( std::find(flags.begin(), flags.end(), "empty") != flags.end() ) ||
                                   ( std::find(flags.begin(), flags.end(), "e") != flags.end() );

            bool shouldPrintHeader = ( std::find(flags.begin(), flags.end(), "header") != flags.end() ) ||
                                     ( std::find(flags.begin(), flags.end(), "h") != flags.end() );

            bool shouldPrintOnlyHeader = ( std::find(flags.begin(), flags.end(), "headeronly") != flags.end() ) ||
                                         ( std::find(flags.begin(), flags.end(), "o") != flags.end() );

            if (shouldPrintOnlyHeader) {
                std::cout << tomLabel << " " << tomSize << "\n";
                return;
            }

            if (shouldPrintHeader) {
                std::cout << tomLabel << " " << tomSize << "\n";
            }

            for (int i = 0; i < fileRecordsCount; ++i) {
                FileRecord fileRecord = fileRecords[i];

                if (shouldPrintFree) {
                    std::cout << static_cast<uint16_t>(fileRecord.recordType) << " "
                              << fileRecord.fileName << " "
                              << fileRecord.blockCount;
                }
                else {
                    if (fileRecord.recordType != RecordType::FREE) {
                        std::cout << static_cast<uint16_t>(fileRecord.recordType) << " "
                                  << fileRecord.fileName << " "
                                  << fileRecord.blockCount;
                    }
                }
            }

    }
};

#endif //FS_FULL_HPP
