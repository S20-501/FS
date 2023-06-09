#include <iostream>
#include <fstream>

#include "Monitor.h"

#include "commands/FSCommands.hpp"
#include "serializer/BinSerializer.hpp"

#include "exceptions/FilesystemException.hpp"
#include "exceptions/FileWriteException.hpp"

int main() {
    BinSerializer serializer;

    auto filesystem = Filesystem(serializer);

    try {
        filesystem.open("fs.bin");
    } catch (FilesystemException &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::bad_alloc &e){
        std::cerr << "Not enough memory." << std::endl;
        return -1;
    } catch (FileWriteException &e) {
        std::cerr << "Can't save filesystem state. Check permissions or free space on a disk." << std::endl;
    }

    auto commandFactory = FSCommands(filesystem);

#ifdef MONITOR_WITHFILE
    std::ifstream istream("input.txt");
    if(!istream.is_open()){
        std::cerr << "Can't find input file: input.txt" << std::endl;
        filesystem.close();
        return -7;
    }

    std::ofstream ostream("output.txt");
    if(!ostream.is_open()){
        std::cerr << "Can't find output file: output.txt" << std::endl;
        filesystem.close();
        return -8;
    }

    Monitor monitor(commandFactory, istream, ostream, true);
#else //MONITOR_WITHFILE
    Monitor monitor(commandFactory, std::cin, std::cout, false);
#endif //MONITOR_WITHFILE

    monitor.setPrompt("FS> ");
    monitor.setExitMessage("change da world\nmy final message. Goodb ye.\n");

    try {
        monitor.run();
    } catch (std::bad_alloc &e){
        std::cerr << "Not enough memory." << std::endl;
        filesystem.close();
        return -1;
    } catch (FilesystemException &e) {
        std::cerr << e.what() << std::endl;
        filesystem.close();
        return -2;
    } catch (FileWriteException &e) {
        std::cerr << "Can't save filesystem state. Check permissions or free space on a disk." << std::endl;
        filesystem.close();
        return -3;
    }

    filesystem.close();

    return 0;
}
