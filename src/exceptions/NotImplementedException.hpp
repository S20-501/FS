//
// Created by evezers on 14/05/23.
//

#ifndef FS_NOTIMPLEMENTEDEXCEPTION_HPP
#define FS_NOTIMPLEMENTEDEXCEPTION_HPP

class NotImplementedException : public std::exception{
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Function is not implemented.";
    }
};

#endif //FS_NOTIMPLEMENTEDEXCEPTION_HPP
