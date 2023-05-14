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
    return ""; // always correct
}

std::string Empty::run() {
    // return fs_blackhole();
    std::stringstream stream;

    int countOfFreeBlocks = 0;
    int maxFileSizeCreationLimit = 0;
    int availableFileRecordsCount = 0;
    std::map<int, int> availableSizeBlocks;

    uint16_t segments_count = filesystem.filesystemInfo.segmentsCount;
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment segment = filesystem.filesystemSegment[i];

        uint16_t j = 0;
        while (true) {
            FileRecord fileRecord = segment.fileRecord[j];

            if (fileRecord.recordType == RecordType::FREE) {
                int blockCount = fileRecord.blockCount;

                auto itFind = availableSizeBlocks.find(blockCount);
                if (itFind != availableSizeBlocks.end()) {
                    int& count = itFind->second;
                    ++count;
                } else {
                    availableSizeBlocks.insert( std::pair<int, int>(blockCount, 1) );
                }

                countOfFreeBlocks += blockCount;
                ++availableFileRecordsCount;
                if (blockCount > maxFileSizeCreationLimit) {
                    maxFileSizeCreationLimit = blockCount;
                }
            }

            if (fileRecord.recordType == RecordType::RECORDS_END) {
                break;
            }
            ++j;
        }
    }

    stream << "Free blocks: " << countOfFreeBlocks << "\n";
    stream << "Disk size: " << filesystem.filesystemInfo.blocksCount << "\n";
    stream << "Segments count: " << filesystem.filesystemInfo.segmentsCount << "\n";
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment segment = filesystem.filesystemSegment[i];
        int recordsCount = 0;

        uint16_t j = 0;
        while (true) {
            FileRecord fileRecord = segment.fileRecord[j];

            if (fileRecord.recordType != RecordType::FREE) {
                ++recordsCount;
            }

            if (fileRecord.recordType == RecordType::RECORDS_END) {
                break;
            }
            ++j;
        }
        stream << "Segment " << (i + 1) << ", records count: " << recordsCount << "/" << segment.segmentHeader.segmentsUsed << "\n";
    }

    stream << "File size creation limit: " << maxFileSizeCreationLimit << "\n";
    stream << "Available file records: " << availableFileRecordsCount << "\n";

    stream << "Available size blocks:" << "\n";
    for (const auto &[key, value]: availableSizeBlocks) {
        stream << "\t" << key << ": " << value << "\n";
    }

    return stream.str();
}

std::string Empty::help() {
    return "empty help";
}
