//
// Created by evezers on 14/05/23.
//

#ifndef FS_INVALIDFILEFORMATEXCEPTION_HPP
#define FS_INVALIDFILEFORMATEXCEPTION_HPP


class InvalidFileFormatException : public std::exception{
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Invalid file format.";
    }
};


#endif //FS_INVALIDFILEFORMATEXCEPTION_HPP
