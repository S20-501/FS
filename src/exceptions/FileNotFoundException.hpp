//
// Created by evezers on 11/05/23.
//

#ifndef FS_FILENOTFOUNDEXCEPTION_HPP
#define FS_FILENOTFOUNDEXCEPTION_HPP

#include "IOException.hpp"

class FileNotFoundException : public IOException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "File not found.";
    }
};

#endif //FS_FILENOTFOUNDEXCEPTION_HPP
