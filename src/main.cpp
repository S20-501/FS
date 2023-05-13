#include <iostream>
#include <fstream>

#include "Monitor.h"

#include "commands/FSCommands.hpp"
#include "BinSerializer.hpp"

#include "exceptions/FilesystemNotInitializedException.hpp"

//#define MONITOR_WITHFILE

int main() {
    BinSerializer serializer;

    auto filesystem = Filesystem(serializer);

    try {
        filesystem.open("fs.bin");
    } catch (FilesystemNotInitializedException &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::bad_alloc &e){
        std::cerr << "Not enough memory." << std::endl;
        return -1;
    }

    auto commandFactory = FSCommandFactory<std::tuple<
        Init
//        Full,
//        Empty,
//        Enter,
//        Copy,
//        Move,
//        Del,
//        Squeeze,
//        Help
    >> (filesystem);

#ifdef MONITOR_WITHFILE
    std::ifstream istream("input.txt");
    if(!istream.is_open()){
        throw std::runtime_error("can't find input file");
    }

    std::ofstream ostream("output.txt");
    if(!ostream.is_open()){
        throw std::runtime_error("can't find output file");
    }

    Monitor monitor(commandFactory, istream, ostream, true);
#else //MONITOR_WITHFILE
    Monitor monitor(commandFactory, std::cin, std::cout, false);
#endif //MONITOR_WITHFILE

    try {
        monitor.run();
    } catch (std::bad_alloc &e){
        std::cerr << "Not enough memory." << std::endl;
        filesystem.close();
        return -1;
    }

    filesystem.close();

    return 0;
}
