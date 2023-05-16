#include <sstream>

#include "Squeeze.h"
#include "../UtilsFunctions.hpp"

#include "../dto/FileRecord.hpp"
#include "../dto/FilesystemInfo.hpp"
#include "../dto/FilesystemSegment.hpp"

std::string Squeeze::getQuery(){
    return "squeeze";
}

std::string Squeeze::checkAndAssemble(Parser &parser) {
    return ""; // always correct
}

std::string Squeeze::run() {
    // return fs_squeeze();

    uint16_t segments_count = filesystem.filesystemInfo.segmentsCount;
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment& segment = filesystem.filesystemSegment[i];

        int currentFreeRecordIndex = -1;

        for (int j = 0; j < FilesystemSegment::FILE_RECORDS_COUNT; ++j) {
            FileRecord& fileRecord = segment.fileRecord[j];

            if (fileRecord.recordType == RecordType::FREE) {
                if (currentFreeRecordIndex == -1) {
                    currentFreeRecordIndex = j;
                }
                fileRecord.blockCount = 0;
            }
            else if (fileRecord.recordType == RecordType::RECORDS_END) {
                if (currentFreeRecordIndex != -1) {
                    FileRecord& currentFreeRecord = segment.fileRecord[currentFreeRecordIndex];
                    std::swap(fileRecord, currentFreeRecord);
                    fileRecord.blockCount = 0;
                    fileRecord.recordType = RecordType::RECORDS_END;
                }
                break;
            }
            else {  //not free or end => one of common files
                if (currentFreeRecordIndex != -1) {
                    std::swap(fileRecord, segment.fileRecord[currentFreeRecordIndex]);
                    fileRecord.blockCount = 0;

                    int candidateFreeRecordIndex = currentFreeRecordIndex + 1;
                    if (segment.fileRecord[candidateFreeRecordIndex].recordType == RecordType::FREE) {
                        currentFreeRecordIndex = candidateFreeRecordIndex;
                    } else {
                        currentFreeRecordIndex = j;
                    }
                }
            }
        }
    }

    filesystem.serializer.save(filesystem);
    std::stringstream stream;
    stream << "squeeze command executed";
    return stream.str();
}

std::string Squeeze::help() {
    return "squeeze help";
}
