//
// Created by evezers on 01/05/23.
//

#ifndef FS_FILESYSTEM_HPP
#define FS_FILESYSTEM_HPP

#include <fstream>
#include <cstring>
#include "FilesystemInfo.hpp"
#include "FileRecord.hpp"
#include "FilesystemSegment.hpp"

typedef std::uint8_t byte;

//struct block{
//    byte blk[512];
//};
//
//
//struct segment{
//    byte blk[1024];
//};


//struct word{
//    byte blk[2];
//};

//const size_t BLOCKS_IN_SEGMENT = 2;
//
//const size_t segmentsCount = 10;

class Filesystem {
private:
    std::fstream file;


    static constexpr byte FILESYSTEM_INFO_START_BLOCK = 1;
    static constexpr byte SEGMENTS_START_BLOCK = 6;
    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;
    static constexpr uint16_t BLOCK_SIZE = 512;

public:
    FilesystemInfo filesystemInfo {};
    FilesystemSegment *filesystemSegment = nullptr;

public:
    Filesystem() : file() {}

    Filesystem(Filesystem &filesystem) = delete;

    Filesystem(Filesystem &&filesystem) noexcept : file() {
        file = std::move(filesystem.file);

        filesystemSegment = filesystem.filesystemSegment;
        filesystem.filesystemSegment = nullptr;
    }

    Filesystem &operator =(const Filesystem &filesystem) = delete;

    Filesystem &operator =(Filesystem &&filesystem) noexcept {
        file = std::move(filesystem.file);

        filesystemSegment = filesystem.filesystemSegment;
        filesystem.filesystemSegment = nullptr;

        return *this;
    }

    void init(byte blocks, byte segments, const char *label = "DEFAULT"){
        file.open("fs.bin", std::ios_base::out);

        if (!file.is_open()) {
            throw std::exception();
        }

        filesystemSegment = new FilesystemSegment[segments];
        // TODO: not enough memory


        uint16_t fsHeaderSize = SEGMENTS_START_BLOCK + segments + SEGMENT_LENGTH_IN_BLOCKS;
        uint16_t currentSegmentFilesStart = fsHeaderSize;
        uint16_t blocksPerSegment = blocks / segments;


        filesystemInfo.segmentsCount = segments;
        filesystemInfo.blocksCount = blocks;
        strncpy(filesystemInfo.volumeLabel, label, 11);

        file.seekp(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
        file.write(reinterpret_cast<const char *>(&filesystemInfo), sizeof(filesystemInfo));
        // TODO: not enough memory


        for (int i = 0; i < segments; i++){
//            filesystemSegment->segmentHeader.

            filesystemSegment[i].fileRecord[0].recordType = RECORDS_END;
            filesystemSegment[i].segmentHeader.segmentsCount = segments;
            filesystemSegment[i].segmentHeader.filesStart = currentSegmentFilesStart;

            currentSegmentFilesStart += blocksPerSegment;

            for (int j = 0; j < 63; ++j) {
                filesystemSegment[i].fileRecord[j].blockCount = static_cast<uint16_t>(j);
            }

            file.seekp((SEGMENTS_START_BLOCK + i) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
            file.write(reinterpret_cast<const char *>(&filesystemSegment[i]), sizeof(filesystemSegment[i]));
            // TODO: not enough memory

        }

    }

    void open(){
        file.open("fs.bin");

        if (!file.is_open()) {
            throw "File mnort founfd, please init";
        }


        file.seekg(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&filesystemInfo), sizeof(filesystemInfo));

        if (filesystemInfo.segmentsCount > 31) {
            throw "e";
        }

        filesystemSegment = new FilesystemSegment[filesystemInfo.segmentsCount];
        // TODO: not enough memory


        for (int i = 0; i < filesystemInfo.segmentsCount; i++){
            file.seekg((SEGMENTS_START_BLOCK + i) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
            file.read(reinterpret_cast<char *>(&filesystemSegment[i]), sizeof(filesystemSegment[i]));
            // TODO: unexpected end of file
        }

        file.close();
    }

    void close(){
        file.close();
    }
};


#endif //FS_FILESYSTEM_HPP
