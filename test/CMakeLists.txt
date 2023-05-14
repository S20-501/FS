enable_testing()

cmake_policy(SET CMP0135 NEW)

# ----------GTest Connect
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include(GoogleTest)
# ----------GTest Connect

add_executable(
        FS_tests
        commandFactoryTest.cpp
)

target_link_libraries(
        FS_tests
        GTest::gtest_main
        monitor
        FSCommands
        FS
)

gtest_discover_tests(FS_tests)
