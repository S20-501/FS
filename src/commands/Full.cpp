#include <sstream>
#include <iomanip>

#include "Full.h"
#include "../UtilsFunctions.hpp"

#include "../dto/FileRecord.hpp"
#include "../dto/FilesystemInfo.hpp"
#include "../dto/FilesystemSegment.hpp"


std::string Full::getQuery(){
    return "full";
}

std::string Full::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    setEmpty(parser.getBoolArgs());
    setHeader(parser.getBoolArgs());
    setHeaderonly(parser.getBoolArgs());

    return errorMessage;
}

std::string Full::checkAmount(const Parser &parser) {
    if(parser.getBoolArgs().size() > 3){
        return WRONGBOOLSAMOUNT;
    }

    return "";
}

void Full::setEmpty(const boolArgs_t &bools) {
    UtilsFunctions::findAndSetBoolArg(bools, empty, "empty", "e");
}

void Full::setHeader(const boolArgs_t &bools) {
    UtilsFunctions::findAndSetBoolArg(bools, header, "header", "h");
}

void Full::setHeaderonly(const boolArgs_t &bools) {
    UtilsFunctions::findAndSetBoolArg(bools, headeronly, "headeronly", "o");
}

std::string Full::run() {
    // return fs_full(empty, header, headeronly);
    bool shouldPrintFree = empty;
    bool shouldPrintHeader = header;
    bool shouldPrintOnlyHeader = headeronly;

    std::stringstream ss;

    char* tomLabel = filesystem.filesystemInfo.volumeLabel;
    uint16_t tomSize = filesystem.filesystemInfo.blocksCount;

    if (shouldPrintOnlyHeader) {
        ss << tomLabel << " " << tomSize << "\n";
        return ss.str();
    }

    if (shouldPrintHeader) {
        ss << tomLabel << " " << tomSize << "\n";
    }

    uint16_t segments_count = filesystem.filesystemInfo.segmentsCount;
    for (uint16_t i = 0; i < segments_count; ++i) {
        FilesystemSegment segment = filesystem.filesystemSegment[i];

        for (auto fileRecord : segment.fileRecord) {
            if (fileRecord.recordType == RecordType::RECORDS_END) {
                ss << std::setw(6) << std::setfill('0') << std::oct << fileRecord.recordType << " ";
                ss << "end " << std::dec << fileRecord.blockCount << "\n";
                break;
            }

            if (shouldPrintFree) {
                ss << std::setw(6) << std::setfill('0') << std::oct << fileRecord.recordType << " ";
                ss << fileRecord.fileName << " " << std::dec << fileRecord.blockCount << "\n";
            }
            else {
                if (fileRecord.recordType != RecordType::FREE) {
                    ss << std::setw(6) << std::setfill('0') << std::oct << fileRecord.recordType << " ";
                    ss << fileRecord.fileName << " " << std::dec << fileRecord.blockCount << "\n";
                }
            }

        }
    }

    return ss.str();
}

std::string Full::help() {
    return "full help";
}
