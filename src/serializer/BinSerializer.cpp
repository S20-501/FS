//
// Created by evezers on 02/05/23.
//

#include "BinSerializer.hpp"
#include "exceptions/UnexpectedEndOfFileException.hpp"
#include "exceptions/InvalidFileFormatException.hpp"
#include "exceptions/NotImplementedException.hpp"
#include "exceptions/FileWriteException.hpp"
#include "exceptions/FileCannotCreate.hpp"

void BinSerializer::open(const std::string &filename) {
    file.open(filename);

    if (!file.is_open()) {
        throw FileNotFoundException();
    }
}

void BinSerializer::close() {
    file.close();
}

void BinSerializer::load(Filesystem &filesystem) {
    load(filesystem.filesystemInfo);

    filesystem.filesystemSegment = new FilesystemSegment[filesystem.filesystemInfo.segmentsCount];

    for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
        load(filesystem.filesystemSegment[i], i);
    }
}

void BinSerializer::load(FilesystemInfo &filesystemInfo) {
    file.seekg(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&filesystemInfo), sizeof(filesystemInfo));

    if (file.gcount() != sizeof(filesystemInfo)){
        throw UnexpectedEndOfFileException();
    }

    if (filesystemInfo.segmentsCount > 31) {
        throw InvalidFileFormatException();
    }
}

void BinSerializer::load(FilesystemSegment &filesystemSegment, off_t segmentNumber) {
    file.seekg((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&filesystemSegment), sizeof(filesystemSegment));

    if (file.gcount() != sizeof(filesystemSegment)){
        throw UnexpectedEndOfFileException();
    }
}

void BinSerializer::load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) {
    (void) fileRecord;
    (void) segmentNumber;
    (void) fileNumber;

    throw NotImplementedException();
}

void BinSerializer::save(Filesystem &filesystem) {
    file.clear();
    // TODO: clear error bits in other place of code
    save(filesystem.filesystemInfo);

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

    if (file.bad()){
        throw FileWriteException();
    }
}

void BinSerializer::save(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) {
    (void) fileRecord;
    (void) segmentNumber;
    (void) fileNumber;

    throw NotImplementedException();
}

bool BinSerializer::is_open() {
    return file.is_open();
}

void BinSerializer::create(const std::string &filename) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        throw FileCannotCreate();
    }
}
