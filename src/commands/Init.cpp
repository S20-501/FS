#include <sstream>
#include <cstring>

#include "Init.h"
#include "exceptions/FileCannotCreate.hpp"
#include "../CommonFunctions/CommonFunctions.h"

std::string Init::getQuery(){
    return "init";
}

std::string Init::checkAndAssemble(Parser &parser) {
    std::string errorMessage;

    if(errorMessage = checkAmount(parser); !errorMessage.empty()) return errorMessage;

    if(errorMessage = setBlocks(parser.getKeyArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setSegments(parser.getKeyArgs()); !errorMessage.empty()) return errorMessage;
    if(errorMessage = setLabel(parser.getPosArgs()); !errorMessage.empty()) return errorMessage;

    return errorMessage;
}

std::string Init::checkAmount(const Parser &parser) {
    if(parser.getKeyArgs().size() > 2){
        return TOO_MANY_ARGS;
    }

    if(parser.getKeyArgs().size() < 2){
        return NOT_ENOUGH_ARGS;
    }

    if(parser.getPosArgs().size() > 1){
        return TOO_MANY_ARGS;
    }

    return "";
}

std::string Init::setBlocks(const keyArgs_t &keys) {
    if(auto it = keys.find("blocks"); it != keys.end() || ((it = keys.find("b")) != keys.end())) {
        // convert to int
        int intBlocks = 0;
        if(MonCom::convertToNumber(it->second, intBlocks)) return BLOCKS_CANT_CONVERT;

        // check restrictions
        if (intBlocks < 1 || 65535 < intBlocks) {
            return BLOCKS_RESTRICTED;
        }

        blocks = static_cast<uint16_t>(intBlocks);
    } else {
        return NO_BLOCKS_VALUE;
    }

    return "";
}

std::string Init::setSegments(const keyArgs_t &keys) {
    if(auto it = keys.find("segments"); it != keys.end() || ((it = keys.find("s")) != keys.end())){
        // convert to int
        int intSegments = 0;
        if (MonCom::convertToNumber(it->second, intSegments)) return SEGMENTS_CANT_CONVERT;

        // check restrictions
        if(intSegments < 1 || 31 < intSegments){
            return SEGMENTS_RESTRICTED;
        }

        segments = static_cast<uint16_t>(intSegments);
    } else {
        return NO_SEGMENTS_VALUE;
    }

    return "";
}

std::string Init::setLabel(posArgs_t &poss) {
    if (poss.empty()){
        label = DEFAULT_LABEL;
    } else {
        label = std::move(poss.back());
        poss.pop_back();
    }

    if(!MonCom::isASCII(label)){
        return LABEL_INCORRECT;
    }

    if(label.size() > 10){
        return LABEL_TOO_LONG;
    }

    return "";
}

std::string Init::run() {
    if (!filesystem.serializer.is_open()){
        filesystem.serializer.create("fs.bin");
    }

    static constexpr byte SEGMENTS_START_BLOCK = 6;
    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;

    if (filesystem.filesystemInfo.segmentsCount != 0) delete[] filesystem.filesystemSegment;

    filesystem.filesystemSegment = new FilesystemSegment[segments];

    uint16_t fsHeaderSize = SEGMENTS_START_BLOCK + segments + SEGMENT_LENGTH_IN_BLOCKS;
    uint16_t currentSegmentFilesStart = fsHeaderSize;
    uint16_t blocksPerSegment = blocks / segments;


    filesystem.filesystemInfo.segmentsCount = segments;
    filesystem.filesystemInfo.blocksCount = blocks;
    strncpy(filesystem.filesystemInfo.volumeLabel, label.c_str(), 11);

    for (int i = 0; i < segments; i++){
        filesystem.filesystemSegment[i].fileRecord[0].recordType = RECORDS_END;
        filesystem.filesystemSegment[i].segmentHeader.segmentsCount = segments;
        filesystem.filesystemSegment[i].segmentHeader.filesStart = currentSegmentFilesStart;

        currentSegmentFilesStart += blocksPerSegment;

        for (int j = 0; j < 63; ++j) {
            filesystem.filesystemSegment[i].fileRecord[j].blockCount = static_cast<uint16_t>(j);
        }
    }

    filesystem.serializer.save(filesystem);

    std::stringstream str;
    str << "Filesystem created successfully.";
    return str.str();
}

std::string Init::help() {
    return "usage: init -b <blocks> -s <segments> \"LABEL\"";
}

std::string Init::processQuery(Parser &parser) {
    auto poss = parser.getBoolArgs();
    std::string resultMessage;

    if(std::find(std::begin(poss), std::end(poss), "help") != std::end(poss)){
        return help();
    } else if(resultMessage = checkAndAssemble(parser); resultMessage.empty()) { // parser becomes invalid here, if no error
        resultMessage = run();
    } else{
        resultMessage = COMMON_ERROR_MESSAGE + resultMessage;
    }

    return resultMessage;
}
