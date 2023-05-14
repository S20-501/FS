//
// Created by evezers on 14/05/23.
//

#ifndef FS_UNEXPECTEDENDOFFILEEXCEPTION_HPP
#define FS_UNEXPECTEDENDOFFILEEXCEPTION_HPP

#include "IOException.hpp"

class UnexpectedEndOfFileException : public IOException {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Enexpected end of file.";
    }
};

#endif //FS_UNEXPECTEDENDOFFILEEXCEPTION_HPP
