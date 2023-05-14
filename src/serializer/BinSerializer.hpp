//
// Created by evezers on 02/05/23.
//

#ifndef FS_BINSERIALIZER_HPP
#define FS_BINSERIALIZER_HPP

#include <fstream>
#include "exceptions/FileNotFoundException.hpp"
#include "serializer/ISerializer.hpp"

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

    void open(const std::string& filename) override;
    void create(const std::string &filename) override;
    void close() override;

    bool is_open() override;

    void load(Filesystem &filesystem) override;
    void load(FilesystemInfo &filesystemInfo) override;
    void load(FilesystemSegment &filesystemSegment, off_t segmentNumber) override;
    void load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) override;

    void save(Filesystem &filesystem) override;
    void save(FilesystemInfo &filesystemInfo) override;
    void save(FilesystemSegment &filesystemSegment, off_t segmentNumber) override;
    void save(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) override;
};

#endif //FS_BINSERIALIZER_HPP
