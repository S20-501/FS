//
// Created by evezers on 01/05/23.
//

#ifndef FS_FILESYSTEM_HPP
#define FS_FILESYSTEM_HPP

#include <string>

#include "dto/FilesystemInfo.hpp"
#include "dto/FilesystemSegment.hpp"
#include "serializer/ISerializer.hpp"
#include "exceptions/FilesystemNotInitializedException.hpp"

class ISerializer;

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

    Filesystem &operator =(Filesystem &&filesystem) noexcept;

    void open(const std::string& filename);

    void close();
};


#endif //FS_FILESYSTEM_HPP
