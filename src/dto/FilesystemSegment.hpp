//
// Created by evezers on 01/05/23.
//

#ifndef FS_FILESYSTEMSEGMENT_HPP
#define FS_FILESYSTEMSEGMENT_HPP

#include "FileRecord.hpp"

class FilesystemSegment {
public:
    struct SegmentHeader{
        uint16_t segmentsCount;
        uint16_t nextSegment;
        uint16_t segmentsUsed;
        uint16_t additionalBytes;
        uint16_t filesStart;
    } segmentHeader {};

    uint8_t reserved[6]{};

    FileRecord fileRecord[63];
};


#endif //FS_FILESYSTEMSEGMENT_HPP
