//
// Created by evezers on 11/05/23.
//

#include <sstream>

#ifndef FS_FILENOTFOUNDEXCEPTION_HPP
#define FS_FILENOTFOUNDEXCEPTION_HPP

class FileNotFoundException : public std::exception{
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "File not found.";
    }
};
#endif //FS_FILENOTFOUNDEXCEPTION_HPP
