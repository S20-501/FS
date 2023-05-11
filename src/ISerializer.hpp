//
// Created by evezers on 02/05/23.
//

#ifndef FS_ISERIALIZER_HPP
#define FS_ISERIALIZER_HPP


#include "Filesystem.hpp"

class Filesystem;

class ISerializer {
public:
    virtual bool open(const std::string& filename) = 0;
    virtual bool create(const std::string& filename) = 0;
    virtual void close() = 0;

    virtual bool is_open() = 0;

    virtual void load(Filesystem &filesystem) = 0;
    virtual void load(FilesystemInfo &filesystemInfo) = 0;
    virtual void load(FilesystemSegment &filesystemSegment, off_t segmentNumber) = 0;
    virtual void load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) = 0;

    virtual void save(Filesystem &filesystem) = 0;
    virtual void save(FilesystemInfo &filesystemInfo) = 0;
    virtual void save(FilesystemSegment &filesystemSegment, off_t segmentNumber) = 0;
    virtual void save(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) = 0;

    virtual ~ISerializer() = default;
};


#endif //FS_ISERIALIZER_HPP
