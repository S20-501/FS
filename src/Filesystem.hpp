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
#include "ISerializer.hpp"
#include "exceptions/FilesystemNotInitializedException.hpp"

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
//    static constexpr byte FILESYSTEM_INFO_START_BLOCK = 1;
//    static constexpr byte SEGMENTS_START_BLOCK = 6;
//    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;
//    static constexpr uint16_t BLOCK_SIZE = 512;

public:
    ISerializer &serializer;
    FilesystemInfo filesystemInfo {};
    FilesystemSegment *filesystemSegment = nullptr;

public:
    explicit Filesystem(ISerializer &serializer) : serializer(serializer) {}

    Filesystem(Filesystem &filesystem) = delete; // can't copy serializer and info about filesystem

    Filesystem(Filesystem &&filesystem) noexcept
    : serializer(filesystem.serializer), filesystemInfo(filesystem.filesystemInfo) {
        filesystemSegment = filesystem.filesystemSegment;
        filesystem.filesystemSegment = nullptr;
    }

    Filesystem &operator =(const Filesystem &filesystem) = delete; // can't copy serializer and info about filesystem

    Filesystem &operator =(Filesystem &&filesystem) noexcept {
        serializer = filesystem.serializer;
        filesystemInfo = filesystem.filesystemInfo;

        filesystemSegment = filesystem.filesystemSegment;
        filesystem.filesystemSegment = nullptr;

        return *this;
    }

    void open(const std::string& filename){
        if (!serializer.open(filename)){
            throw FilesystemNotInitializedException();
        }

        serializer.load(filesystemInfo);

        delete[] filesystemSegment;
        filesystemSegment = new FilesystemSegment[filesystemInfo.segmentsCount];
        // TODO: not enough memory


        for (int i = 0; i < filesystemInfo.segmentsCount; i++){
            serializer.load(filesystemSegment[i], i);
        }
    }

    void close(){
        serializer.close();
    }
};


#endif //FS_FILESYSTEM_HPP
