#include <gtest/gtest.h>

#include "serializer/BinSerializer.hpp"
#include "Filesystem.hpp"
#include "commands/FSCommands.hpp"

namespace{
    TEST(parseCommand, inittype){
        BinSerializer serializer;

        auto filesystem = Filesystem(serializer);

        std::string query = "init";

        auto command = FSCommands(filesystem).getCommand(query);
    }
}
