//
// Created by evezers on 01/05/23.
//

#include "Filesystem.hpp"
#include "exceptions/UnexpectedEndOfFileException.hpp"
#include "exceptions/InvalidFileFormatException.hpp"
#include "exceptions/FilesystemCorruptedException.hpp"
#include "exceptions/FileNotFoundException.hpp"

void Filesystem::open(const std::string &filename) {
    try {
        serializer.open(filename);
    } catch (FileNotFoundException &e){
        throw FilesystemNotInitializedException();
    }

    try {
        serializer.load(filesystemInfo);
    } catch (UnexpectedEndOfFileException &e) {
        throw FilesystemCorruptedException();
    } catch (InvalidFileFormatException &e) {
        throw FilesystemCorruptedException();
    }

    delete[] filesystemSegment;
    filesystemSegment = new FilesystemSegment[filesystemInfo.segmentsCount];

    for (int i = 0; i < filesystemInfo.segmentsCount; i++){
        try {
            serializer.load(filesystemSegment[i], i);
        } catch (UnexpectedEndOfFileException &e) {
            throw FilesystemCorruptedException();
        } catch (InvalidFileFormatException &e) {
            throw FilesystemCorruptedException();
        }
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
