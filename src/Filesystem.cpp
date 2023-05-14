//
// Created by evezers on 01/05/23.
//

#include "Filesystem.hpp"

void Filesystem::open(const std::string &filename) {
    if (!serializer.open(filename)){
        throw FilesystemNotInitializedException();
    }

    serializer.load(filesystemInfo);

    delete[] filesystemSegment;
    filesystemSegment = new FilesystemSegment[filesystemInfo.segmentsCount];

    for (int i = 0; i < filesystemInfo.segmentsCount; i++){
        serializer.load(filesystemSegment[i], i);
    }
}

void Filesystem::close() {
    serializer.close();
}

Filesystem &Filesystem::operator=(Filesystem &&filesystem) noexcept {
    serializer = filesystem.serializer;
    filesystemInfo = filesystem.filesystemInfo;

    filesystemSegment = filesystem.filesystemSegment;
    filesystem.filesystemSegment = nullptr;

    return *this;
}
