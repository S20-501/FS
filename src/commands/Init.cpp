#include <sstream>

#include "Init.h"
#include "../utils/utilFunctions.h"

//Init::Init() = default;

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
    if(parser.getKeyArgs().size() != 2){
        return WRONGKEYSAMOUNT;
    }
    if(parser.getPosArgs().size() > 1){
        return WRONGPOSSAMOUNT;
    }

    return "";
}

std::string Init::setBlocks(const keyArgs_t &keys) {
    if(auto it = keys.find("blocks"); it != keys.end() || ((it = keys.find("b")) != keys.end())) {
        // convert to int
        if(convertToNumber(it->second, blocks)) return BLOCKSCANTCONVERT;

        // check restrictions
        if (blocks < 1 || 65535 < blocks) {
            return BLOCKSRESTRICTED;
        }
    } else {
        return NOBLOCKSVALUE;
    }

    return "";
}

std::string Init::setSegments(const keyArgs_t &keys) {
    if(auto it = keys.find("segments"); it != keys.end() || ((it = keys.find("s")) != keys.end())){
        // convert to int
        if (convertToNumber(it->second, segments)) return SEGMENTSCANTCONVERT;

        // check restrictions
        if(segments < 1 || 31 < segments){
            return SEGMENTSRESTRICTED;
        }
    } else {
        return NOSEGMENTSVALUE;
    }

    return "";
}

std::string Init::setLabel(posArgs_t &poss) {
    label = poss.empty() ? DEFAULTLABEL : std::move(poss.back()); // label is optional
    poss.pop_back();

    if(label.size() > 10 || !isASCII(label)){
        return LABELINCORRECT;
    }

    return "";
}

std::string Init::run() {
    static constexpr byte FILESYSTEM_INFO_START_BLOCK = 1;
    static constexpr byte SEGMENTS_START_BLOCK = 6;
    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;
    static constexpr uint16_t BLOCK_SIZE = 512;

    if (filesystem->filesystemInfo.segmentsCount != 0) delete[] filesystem->filesystemSegment;

    filesystem->filesystemSegment = new FilesystemSegment[segments];
    // TODO: not enough memory


    uint16_t fsHeaderSize = SEGMENTS_START_BLOCK + segments + SEGMENT_LENGTH_IN_BLOCKS;
    uint16_t currentSegmentFilesStart = fsHeaderSize;
    uint16_t blocksPerSegment = blocks / segments;


    filesystem->filesystemInfo.segmentsCount = segments;
    filesystem->filesystemInfo.blocksCount = blocks;
    strncpy(filesystem->filesystemInfo.volumeLabel, label.c_str(), 11);

//    filesystem->serializer.save(filesystem->filesystemInfo);

    for (int i = 0; i < segments; i++){
        filesystem->filesystemSegment[i].fileRecord[0].recordType = RECORDS_END;
        filesystem->filesystemSegment[i].segmentHeader.segmentsCount = segments;
        filesystem->filesystemSegment[i].segmentHeader.filesStart = currentSegmentFilesStart;

        currentSegmentFilesStart += blocksPerSegment;

        for (int j = 0; j < 63; ++j) {
            filesystem->filesystemSegment[i].fileRecord[j].blockCount = static_cast<uint16_t>(j);
        }

//        filesystem->serializer.save(filesystem->filesystemSegment[i], i);
    }



    filesystem->serializer.save(*filesystem);
// TODO

    std::stringstream str;
    str << "FSinit command executed, blocks: \"" << blocks <<
        "\", segments: \"" << segments << "\", label: \"" << label << "\", s: \"" << filesystem->filesystemInfo.volumeLabel << "\"";
    return str.str();
}

std::string Init::help() {
    return "FSinit help\ninit help second line";
}
