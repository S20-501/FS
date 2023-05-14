//
// Created by ilyap on 04.05.2023.
//

#ifndef FS_EMPTY_HPP
#define FS_EMPTY_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Filesystem.hpp"


class Empty{
private:
    Filesystem* filesystem;
    std::vector<std::string> flags;

public:
    Empty(Filesystem& filesystem, std::vector<std::string> flags) {
        this->filesystem = &filesystem;
        this->flags = flags;
    }
    std::string run() {
        bool shouldPrintFree = ( std::find(flags.begin(), flags.end(), "empty") != flags.end() ) ||
                               ( std::find(flags.begin(), flags.end(), "e") != flags.end() );
        std::stringstream ss;

        ss << "Free blocks: " << filesystem->filesystemInfo.blocksCount << "\n";
        // disk size?
        ss << "Segments count: " << filesystem->filesystemInfo.segmentsCount << "\n";
        for (int i = 0; i < filesystem->filesystemInfo.segmentsCount; ++i) {
            ss << "Segment " << (i + 1) << ", " <<
                "records count: " <<
                filesystem->filesystemSegment[i].segmentHeader.segmentsUsed <<
                "/" << filesystem->filesystemSegment[i].segmentHeader.segmentsCount << "\n";
        }

    }
};

#endif //FS_EMPTY_HPP
