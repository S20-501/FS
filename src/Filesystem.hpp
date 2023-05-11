//
// Created by evezers on 01/05/23.
//

#ifndef FS_FILESYSTEM_HPP
#define FS_FILESYSTEM_HPP

#include "FilesystemInfo.hpp"
#include "FilesystemSegment.hpp"
#include "ISerializer.hpp"
#include "exceptions/FilesystemNotInitializedException.hpp"

class Filesystem {
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

        for (int i = 0; i < filesystemInfo.segmentsCount; i++){
            serializer.load(filesystemSegment[i], i);
        }
    }

    void close(){
        serializer.close();
    }
};


#endif //FS_FILESYSTEM_HPP
