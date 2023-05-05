//
// Created by evezers on 02/05/23.
//

#ifndef FS_INIT_HPP
#define FS_INIT_HPP

#include "../Filesystem.hpp"
#include "Init.h"

namespace FS{
    class Init : public IFilesystemBaseCommand {
        Filesystem &fs;
        byte blocks;
        byte segments;
        const char *label;

    public:
        constexpr static std::string query = "init";

        Init(Filesystem &fs, byte blocks, byte segments, const char *label)
            : fs(fs), blocks(blocks), segments(segments), label(label) {}

        std::string execute() override {
            std::stringstream ss;

            ss << "owner: " << fs.filesystemInfo.owner  << std::endl;

            ss << "test label: " << label << std::endl;

            return ss.str();
        }

        bool validate() override {

        }
    };
}

#endif //FS_INIT_HPP
