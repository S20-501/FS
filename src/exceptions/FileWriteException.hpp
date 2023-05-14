//
// Created by evezers on 14/05/23.
//

#ifndef FS_FILEWRITEEXCEPTION_HPP
#define FS_FILEWRITEEXCEPTION_HPP

#include "IOException.hpp"

class FileWriteException : public IOException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Couldn't write to file.";
    }
};

#endif //FS_FILEWRITEEXCEPTION_HPP
