//
// Created by evezers on 14/05/23.
//

#ifndef FS_FILESYSTEMCORRUPTEDEXCEPTION_HPP
#define FS_FILESYSTEMCORRUPTEDEXCEPTION_HPP

#include "FilesystemException.hpp"

class FilesystemCorruptedException : public FilesystemException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Filesystem corrupted. Please run init command.";
    }
};

#endif //FS_FILESYSTEMCORRUPTEDEXCEPTION_HPP
