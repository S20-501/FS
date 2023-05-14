//
// Created by evezers on 02/05/23.
//

#include "BinSerializer.hpp"

bool BinSerializer::open(const std::string &filename) {
    file.open(filename);

    if (!file.is_open()) {
        return false;
    }

    return true;
}

void BinSerializer::close() {
    file.close();
}

void BinSerializer::load(Filesystem &filesystem) {
    load(filesystem.filesystemInfo);

    filesystem.filesystemSegment = new FilesystemSegment[filesystem.filesystemInfo.segmentsCount];

    for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
        load(filesystem.filesystemSegment[i], i);
        // TODO: unexpected end of file
    }
}

void BinSerializer::load(FilesystemInfo &filesystemInfo) {
    file.seekg(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&filesystemInfo), sizeof(filesystemInfo));

    if (filesystemInfo.segmentsCount > 31) {
        // TODO: unexpected end of file
        throw "e";
    }
}

void BinSerializer::load(FilesystemSegment &filesystemSegment, off_t segmentNumber) {
    file.seekg((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&filesystemSegment), sizeof(filesystemSegment));
    // TODO: unexpected end of file
}

void BinSerializer::load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) {
    (void) fileRecord;
    (void) segmentNumber;
    (void) fileNumber;

    throw "Not implemented";
}

void BinSerializer::save(Filesystem &filesystem) {
    file.clear();
    // TODO: clear error bits in other place of code
    save(filesystem.filesystemInfo);

    // TODO: not enough memory

    for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
        save(filesystem.filesystemSegment[i], i);
    }

    file.flush();
}

void BinSerializer::save(FilesystemInfo &filesystemInfo) {
    file.seekp(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
    file.write(reinterpret_cast<const char *>(&filesystemInfo), sizeof(filesystemInfo));
}

void BinSerializer::save(FilesystemSegment &filesystemSegment, off_t segmentNumber) {
    file.seekp((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
    file.write(reinterpret_cast<const char *>(&filesystemSegment), sizeof(filesystemSegment));
    // TODO: not enough memory
}

void BinSerializer::save(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) {
    (void) fileRecord;
    (void) segmentNumber;
    (void) fileNumber;

    throw "Not implemented";
}

bool BinSerializer::is_open() {
    return file.is_open();
}

bool BinSerializer::create(const std::string &filename) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        return false;
    }

    return true;
}
