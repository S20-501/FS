#include <sstream>
#include <map>

#include "Empty.h"
#include "CommonFunctions.h"
#include "UtilsFunctions.hpp"

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
        return TOO_MANY_ARGS;
    }

    return "";
}


std::string Empty::run() {
    std::stringstream stream;

    int countOfFreeBlocks = 0;
    int maxFileSizeCreationLimit = 0;
    int availableFileRecordsCount = 0;
    std::map<int, int> availableSizeBlocks;  //key - blockCount, value - count of files with blockCount key

    uint16_t segments_count = filesystem.filesystemInfo.segmentsCount;
    for (uint16_t currentSegmentIndex = 0; currentSegmentIndex < segments_count; ++currentSegmentIndex) {
        FilesystemSegment currentSegment = filesystem.filesystemSegment[currentSegmentIndex];
        int countOfBusyBlocksInSegment = 0;
        int countOfFreeBlocksInSegment = 0;

        for (int currentRecordIndex = 0; currentRecordIndex < FilesystemSegment::FILE_RECORDS_COUNT; ++currentRecordIndex) {
            FileRecord fileRecord = currentSegment.fileRecord[currentRecordIndex];

            int countOfFreeBlocksToAdd;
            int countOfAvailableRecordsToAdd;

            if ( !(fileRecord.recordType == RecordType::FREE || fileRecord.recordType == RecordType::RECORDS_END) ) {
                countOfBusyBlocksInSegment += fileRecord.blockCount;

                if (currentRecordIndex == FilesystemSegment::FILE_RECORDS_COUNT - 1) {  //last and not end record
                    int segmentSize = UtilsFunctions::getSegmentSizeInBlocks(filesystem,currentSegmentIndex);
                    int countOfFreeBlocksLeft = segmentSize - countOfBusyBlocksInSegment - countOfFreeBlocksInSegment;
                    countOfFreeBlocksToAdd = countOfFreeBlocksLeft;
                    countOfFreeBlocksInSegment += countOfFreeBlocksToAdd;
                    countOfFreeBlocks += countOfFreeBlocksToAdd;
                }

                continue;
            }

            if (fileRecord.recordType == RecordType::FREE) {
                int blockCount = fileRecord.blockCount;
                countOfFreeBlocksToAdd = blockCount;
                countOfAvailableRecordsToAdd = 1;

                if (empty) {
                    for (int k = 1; k <= blockCount; ++k) {
                        updateAvailableSizeBlocks(availableSizeBlocks, k, 1);
                    }
                }

            }

            else {   //END Record
                int segmentSize = UtilsFunctions::getSegmentSizeInBlocks(filesystem,currentSegmentIndex);

                int countOfFreeBlocksEnd = segmentSize - countOfBusyBlocksInSegment - countOfFreeBlocksInSegment;

                int availableFileRecordsCountEnd = FilesystemSegment::FILE_RECORDS_COUNT - currentRecordIndex - 1;
                countOfFreeBlocksToAdd = countOfFreeBlocksEnd;
                countOfAvailableRecordsToAdd = availableFileRecordsCountEnd;
                if (empty) {
                    for (int k = 1; k <= countOfFreeBlocksEnd; ++k) {
                        int recordsToAdd = countOfFreeBlocksEnd / k;
                        int possibleRecordsToAdd = std::min(recordsToAdd, availableFileRecordsCountEnd);

                        updateAvailableSizeBlocks(availableSizeBlocks, k, possibleRecordsToAdd);
                    }
                }
            }

            countOfFreeBlocksInSegment += countOfFreeBlocksToAdd;
            countOfFreeBlocks += countOfFreeBlocksToAdd;
            availableFileRecordsCount += countOfAvailableRecordsToAdd;
            if (countOfFreeBlocksToAdd > maxFileSizeCreationLimit) {
                maxFileSizeCreationLimit = countOfFreeBlocksToAdd;
            }

            if (fileRecord.recordType == RecordType::RECORDS_END) {
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
            if (fileRecord.recordType != RecordType::FREE && fileRecord.recordType != RecordType::RECORDS_END) {
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


    std::string res = stream.str();
    return UtilsFunctions::removeClosingEndl(res);
}

std::string Empty::help() {
    return "usage: empty <--empty|-e>";
}

void Empty::setEmpty(const boolArgs_t &bools) {
    MonCom::findAndSetBoolArg(bools, empty, "empty", "e");
}

void Empty::updateAvailableSizeBlocks(std::map<int, int> &availableSizeBlocks, int keyToAdd, int valueToAdd) {
        auto itFind = availableSizeBlocks.find(keyToAdd);
        if (itFind != availableSizeBlocks.end()) {
            int& count = itFind->second;
            count += valueToAdd;
        } else {
            availableSizeBlocks.insert( std::pair<int, int>(keyToAdd, valueToAdd) );
        }
}

std::string Empty::description() {
    return "prints filesystem statistics";
}

