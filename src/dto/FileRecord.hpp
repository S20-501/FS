//
// Created by evezers on 01/05/23.
//

#ifndef FS_FILERECORD_HPP
#define FS_FILERECORD_HPP

#include <cstdint>

typedef uint16_t word;
typedef uint8_t byte;

enum RecordType : uint16_t {
    TEMP_FILE = 0400,            // запись о временном файле,
    FREE = 01000,                // запись о свободной области на носителе. Размер свободной области в блоках содержится в 5-ом слове, остальные слова не используются,
    REGULAR_FILE = 02000,        // запись о постоянном файле,
    REGULAR_PROTECTED = 0102000, // запись о постоянном файле, защищенном от операции удаления,
    RECORDS_END = 04000,         // признак конца записей в данном сегменте.
};

union FSDate {
    word dateWord;

    class {
    private:
        byte Year : 4;

    public:
        byte Day : 4;
        byte Month : 4;

        [[nodiscard]] byte getYear() const{
            return static_cast<byte>(Year + 1980);
        }

        void setYear(uint16_t newYear){
            Year = (static_cast<byte>((newYear - 1980) & 0xF));
        }
    } parsedDate;
};

class FileRecord {
public:
    RecordType recordType = RECORDS_END; // 1 word
    char fileName[10] = ""; // 5 words
    uint16_t blockCount = 0x0; // 1 word
    FSDate date {.dateWord=0x0}; //1 word
};

#endif //FS_FILERECORD_HPP
