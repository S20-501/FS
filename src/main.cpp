#include <iostream>
#include <fstream>

#include "../Monitor.h"
#include "Filesystem.hpp"

#include "commands/Init.h"
#include "commands/Full.h"
#include "commands/Empty.h"
#include "commands/Enter.h"
#include "commands/Copy.h"
#include "commands/Move.h"
#include "commands/Del.h"
#include "commands/Squeeze.h"
#include "commands/Help.h"
#include "commands/FSCommandFactory.hpp"
#include "BinSerializer.hpp"

//#define MONITOR_WITHFILE

int main() {
    BinSerializer binSerializer;

    auto fs = Filesystem(binSerializer);

    try {
        fs.open("fs.bin");
    } catch (char *e) {
        // TODO: please run init
        std::cerr << e << std::endl;
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
    >>(fs);

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

    monitor.run();

    fs.close();

    return 0;
}
