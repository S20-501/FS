//
// Created by evezers on 02/05/23.
//

#ifndef FS_BINSERIALIZER_HPP
#define FS_BINSERIALIZER_HPP


class BinSerializer : public ISerializer{
private:
    std::fstream file;

    static constexpr byte FILESYSTEM_INFO_START_BLOCK = 1;
    static constexpr byte SEGMENTS_START_BLOCK = 6;
    static constexpr byte SEGMENT_LENGTH_IN_BLOCKS = 2;
    static constexpr uint16_t BLOCK_SIZE = 512;
public:
//    Filesystem(ISerializer &serializer) : serializer(serializer) {}
//
//    Filesystem(Filesystem &filesystem) = delete;
//
//    Filesystem(Filesystem &&filesystem) noexcept : file() {
////        file = std::move(filesystem.file);
//
//        filesystemSegment = filesystem.filesystemSegment;
//        filesystem.filesystemSegment = nullptr;
//    }
//
//    Filesystem &operator =(const Filesystem &filesystem) = delete;
//
//    Filesystem &operator =(Filesystem &&filesystem) noexcept {
//        file = std::move(filesystem.file);
//
//        filesystemSegment = filesystem.filesystemSegment;
//        filesystem.filesystemSegment = nullptr;
//
//        return *this;
//    }


    bool open(const std::string& filename) override{
        file.open(filename);

        if (!file.is_open()) {
            throw "File mnort founfd, please init";
        }
    }

    void close() override{
        file.close();
    }

    void load(Filesystem &filesystem) override{
        load(filesystem.filesystemInfo);

        filesystem.filesystemSegment = new FilesystemSegment[filesystem.filesystemInfo.segmentsCount];
        // TODO: not enough memory

        for (int i = 0; i < filesystem.filesystemInfo.segmentsCount; i++){
            load(filesystem.filesystemSegment[i], i);
            // TODO: unexpected end of file
        }
    }

    void load(FilesystemInfo &filesystemInfo) override{
        file.seekg(FILESYSTEM_INFO_START_BLOCK * BLOCK_SIZE, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&filesystemInfo), sizeof(filesystemInfo));

        if (filesystemInfo.segmentsCount > 31) {
            throw "e";
        }
    }

    void load(FilesystemSegment &filesystemSegment, off_t segmentNumber) override{
        file.seekg((SEGMENTS_START_BLOCK + segmentNumber) * SEGMENT_LENGTH_IN_BLOCKS * BLOCK_SIZE, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&filesystemSegment), sizeof(filesystemSegment));
        // TODO: unexpected end of file
    }

    void load(FileRecord &fileRecord, off_t segmentNumber, off_t fileNumber) override{
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
        throw "Not implemented";
    }
};


#endif //FS_BINSERIALIZER_HPP
