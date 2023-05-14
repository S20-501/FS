//
// Created by evezers on 14/05/23.
//

#ifndef FS_INVALIDFILEFORMATEXCEPTION_HPP
#define FS_INVALIDFILEFORMATEXCEPTION_HPP

#include "IOException.hpp"

class InvalidFileFormatException : public IOException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Invalid file format.";
    }
};

#endif //FS_INVALIDFILEFORMATEXCEPTION_HPP
