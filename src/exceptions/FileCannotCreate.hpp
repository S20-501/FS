//
// Created by evezers on 11/05/23.
//

#ifndef FS_FILECANNOTCREATE_HPP
#define FS_FILECANNOTCREATE_HPP


class FileCannotCreate : public std::exception {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Cannot create file.";
    }
};


#endif //FS_FILECANNOTCREATE_HPP
