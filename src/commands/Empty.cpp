#include <sstream>
#include <map>

#include "Empty.h"
#include "../UtilsFunctions.hpp"

#include "../dto/FileRecord.hpp"
#include "../dto/FilesystemInfo.hpp"
#include "../dto/FilesystemSegment.hpp"


std::string Empty::getQuery(){
    return "empty";
}

std::string Empty::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    setEmpty(parser.getBoolArgs());

    return errorMessage;
}

std::string Empty::checkAmount(const Parser &parser) {
    if(parser.getBoolArgs().size() > 1){
        return WRONGBOOLSAMOUNT;
    }

    return "";
}

std::string Empty::run() {
    // return fs_blackhole();
    std::stringstream stream;

    int countOfFreeBlocks = 0;
    int maxFileSizeCreationLimit = 0;
    int availableFileRecordsCount = 0;
    std::map<int, int> availableSizeBlocks;  //key - blockCount, value - count of files with blockCount key

    uint16_t segments_count = filesystem.filesystemInfo.segmentsCount;
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment segment = filesystem.filesystemSegment[i];
        int countOfBusyBlocksInSegment = 0;

        for (int j = 0; j < FilesystemSegment::FILE_RECORDS_COUNT; ++j) {
            FileRecord fileRecord = segment.fileRecord[j];

            if (fileRecord.blockCount != 0) {
                countOfBusyBlocksInSegment += fileRecord.blockCount;
            }
            if (fileRecord.recordType == RecordType::FREE) {
                int blockCount = fileRecord.blockCount;

                for (int k = 1; k <= blockCount; ++k) {
                    auto itFind = availableSizeBlocks.find(k);
                    if (itFind != availableSizeBlocks.end()) {
                        int& count = itFind->second;
                        ++count;
                    } else {
                        availableSizeBlocks.insert( std::pair<int, int>(k, 1) );
                    }
                }

                countOfFreeBlocks += blockCount;
                ++availableFileRecordsCount;
                if (blockCount > maxFileSizeCreationLimit) {
                    maxFileSizeCreationLimit = blockCount;
                }
            }

            if (fileRecord.recordType == RecordType::RECORDS_END) {
                int countOfFreeBlocksEnd = filesystem.filesystemInfo.blocksCount / filesystem.filesystemInfo.segmentsCount - countOfBusyBlocksInSegment;
                int availableFileRecordsCountEnd = FilesystemSegment::FILE_RECORDS_COUNT - j - 1;

                for (int k = 1; k <= countOfFreeBlocksEnd; ++k) {
                    int recordsToAdd = countOfFreeBlocksEnd / k;
                    int possibleRecordsToAdd = std::min(recordsToAdd, availableFileRecordsCountEnd);

                    auto itFind = availableSizeBlocks.find(k);
                    if (itFind != availableSizeBlocks.end()) {
                        int& count = itFind->second;
                        count += possibleRecordsToAdd;
                    } else {
                        availableSizeBlocks.insert( std::pair<int, int>(k, possibleRecordsToAdd) );
                    }
                }

                countOfFreeBlocks += countOfFreeBlocksEnd;
                availableFileRecordsCount += availableFileRecordsCountEnd;

                if (countOfFreeBlocksEnd > maxFileSizeCreationLimit) {
                    maxFileSizeCreationLimit = countOfFreeBlocksEnd;
                }
                break;
            }

        }
    }

    stream << "Free blocks: " << countOfFreeBlocks << "\n";
    stream << "Disk size: " << filesystem.filesystemInfo.blocksCount << "\n";
    stream << "Segments count: " << filesystem.filesystemInfo.segmentsCount << "\n";
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment segment = filesystem.filesystemSegment[i];
        int recordsCount = 0;

        for (auto fileRecord : segment.fileRecord) {
            if (fileRecord.recordType != RecordType::FREE) {
                ++recordsCount;
            }

            if (fileRecord.recordType == RecordType::RECORDS_END) {
                break;
            }
        }
        stream << "Segment " << (i + 1) << ", records count: " << recordsCount << "/" << FilesystemSegment::FILE_RECORDS_COUNT << "\n";
    }

    stream << "File size creation limit: " << maxFileSizeCreationLimit << "\n";
    stream << "Available file records: " << availableFileRecordsCount << "\n";

    if (empty) {
        stream << "Available size blocks:" << "\n";
        for (const auto &[key, value]: availableSizeBlocks) {
            stream << "  " << key << ": " << value << "\n";
        }
    }



    return stream.str();
}

std::string Empty::help() {
    return "empty help";
}

void Empty::setEmpty(const boolArgs_t &bools) {
    UtilsFunctions::findAndSetBoolArg(bools, empty, "empty", "e");
}