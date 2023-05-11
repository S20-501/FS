//
// Created by evezers on 11/05/23.
//

#ifndef FS_FILESYSTEMNOTINITIALIZED_HPP
#define FS_FILESYSTEMNOTINITIALIZED_HPP

class FilesystemNotInitializedException : std::exception{
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Filesystem file not found. Please run init command.";
    }
};

#endif //FS_FILESYSTEMNOTINITIALIZED_HPP
