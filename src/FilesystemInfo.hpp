//
// Created by evezers on 01/05/23.
//
#include <cstdint>

#ifndef FS_FILESYSTEMINFO_HPP
#define FS_FILESYSTEMINFO_HPP

class FilesystemInfo{
public:
    char defects[130]{};
    char reserved[2]{};
    char recovery[38]{};
    char reserved2[300]{};
    uint16_t catalogStartBlock = 6;
    uint16_t version = 1;
    char volumeLabel[11] = "DEFAULT";
    char owner[11] = "OWNER";
    char systemName[11] = "SYSTEM";
    uint16_t segmentsCount = 0;
    uint16_t blocksCount = 0;
};

#endif //FS_FILESYSTEMINFO_HPP
