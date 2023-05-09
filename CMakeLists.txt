cmake_minimum_required(VERSION 3.13)
project(FS VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_subdirectory(lib)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
