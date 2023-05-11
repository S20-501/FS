//
// Created by evezers on 02/05/23.
//

#ifndef FS_BINSERIALIZER_HPP
#define FS_BINSERIALIZER_HPP

#include "exceptions/FileNotFoundException.hpp"

class BinSerializer : public ISerializer{
private:
    std::fstream file;

    static constexpr byte FILESYSTEM_INFO_START_BLOCK = 1;
    static constexpr byte SEGMENTS_START_BLOCK = 6;
    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;
    static constexpr uint16_t BLOCK_SIZE = 512;
public:
    BinSerializer() : file() {}

    explicit BinSerializer(const std::string &filename) : file(filename) {}

    BinSerializer(BinSerializer &binSerializer) = delete;

    BinSerializer(BinSerializer &&binSerializer) noexcept : file() {
        file = std::move(binSerializer.file);
    }

    BinSerializer &operator =(const BinSerializer &filesystem) = delete;

    BinSerializer &operator =(BinSerializer &&binSerializer) noexcept {
        file = std::move(binSerializer.file);
        return *this;
    }

    bool open(const std::string& filename) override{
        file.open(filename);

        if (!file.is_open()) {
            return false;
        }

        return true;
    }

    void close() override{
        file.close();
    }

    void load(Filesystem &filesystem) override{
        load(filesystem.filesystemInfo);

        filesystem.filesystemSegment = new FilesystemSegment[filesystem.filesystemInfo.segmentsCount];

        for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
            load(filesystem.filesystemSegment[i], i);
            // TODO: unexpected end of file
        }
    }

    void load(FilesystemInfo &filesystemInfo) override{
        file.seekg(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&filesystemInfo), sizeof(filesystemInfo));

        if (filesystemInfo.segmentsCount > 31) {
            // TODO: unexpected end of file
            throw "e";
        }
    }

    void load(FilesystemSegment &filesystemSegment, off_t segmentNumber) override{
        file.seekg((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&filesystemSegment), sizeof(filesystemSegment));
        // TODO: unexpected end of file
    }

    void load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) override{
        (void) fileRecord;
        (void) segmentNumber;
        (void) fileNumber;

        throw "Not implemented";
    }

    void save(Filesystem &filesystem) override{
        file.clear();
        // TODO: clear error bits in other place of code
        save(filesystem.filesystemInfo);

        // TODO: not enough memory

        for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
            save(filesystem.filesystemSegment[i], i);
        }

        file.flush();
    }

    void save(FilesystemInfo &filesystemInfo) override{
        file.seekp(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
        file.write(reinterpret_cast<const char *>(&filesystemInfo), sizeof(filesystemInfo));
    }

    void save(FilesystemSegment &filesystemSegment, off_t segmentNumber) override{
        file.seekp((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
        file.write(reinterpret_cast<const char *>(&filesystemSegment), sizeof(filesystemSegment));
        // TODO: not enough memory
    }

    void save(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) override{
        (void) fileRecord;
        (void) segmentNumber;
        (void) fileNumber;

        throw "Not implemented";
    }

    bool is_open() override {
        return file.is_open();
    }

    bool create(const std::string &filename) override {
        file.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

        if (!file.is_open()) {
            return false;
        }

        return true;
    }
};


#endif //FS_BINSERIALIZER_HPP
