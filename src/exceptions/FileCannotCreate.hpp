//
// Created by evezers on 11/05/23.
//

#ifndef FS_FILECANNOTCREATE_HPP
#define FS_FILECANNOTCREATE_HPP

#include "IOException.hpp"

class FileCannotCreate : public IOException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Cannot create file.";
    }
};

#endif //FS_FILECANNOTCREATE_HPP
