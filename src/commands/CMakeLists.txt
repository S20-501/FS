#project(FSCommands)

# make library
SET(FScommands_list
        Init.cpp
        Help.cpp
#        Full.cpp
#        Empty.cpp
        Enter.cpp
#        Copy.cpp
#        Move.cpp
#        Del.cpp
#        Squeeze.cpp
        )
add_library(FSCommands STATIC ${FScommands_list})

# make utils visible in cpp
include_directories("${Monitor_SOURCE_DIR}/monitor/commands")

target_link_libraries(FSCommands commands_base monitor_common)
