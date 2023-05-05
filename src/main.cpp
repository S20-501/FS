#include <iostream>
#include <fstream>

#include "Filesystem.hpp"
#include "commands/FilesystemBaseCommandFactory.hpp"
#include "../Monitor.h"
//#include "monitor/CommandFactory.h"


//#define MONITOR_WITHFILE

int main() {
    Filesystem fs = Filesystem();


    ////    try {
////        fs.init(32, 6, "QUAFFFFFFFFFFFFFQQA");
////    } catch (char *e) {
////        std::cerr << e << std::endl;
////    }
//
//    fs.open();

#ifdef MONITOR_WITHFILE
    std::ifstream istream("input.txt");
    if(!istream.is_open()){
        throw std::runtime_error("can't find input file");
    }

    std::ofstream ostream("output.txt");
    if(!ostream.is_open()){
        throw std::runtime_error("can't find output file");
    }

    Monitor monitor(istream, ostream, true);
#else //MONITOR_WITHFILE
    Monitor monitor(std::cin, std::cout, false);
#endif //MONITOR_WITHFILE

    monitor.run();

    return 0;
}