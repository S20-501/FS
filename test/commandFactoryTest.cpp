#include <gtest/gtest.h>

#include "../src/commands/FilesystemBaseCommandFactory.hpp"
#include "../src/commands/Init.hpp"
#include "../src/Filesystem.hpp"


namespace{
    TEST(parseCommand, inittype){
        Filesystem fs;

//        std::ostream &ostream = std::cout;
        std::string query = "init";

        auto command = FS::FilesystemBaseCommandFactory::getCommand(fs, query, 1, 2, "TEST");
//        EXPECT_EQ(command->getCommandType(), CommandsList::INIT);

        command->execute();
    }
}
