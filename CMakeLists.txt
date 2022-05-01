cmake_minimum_required(VERSION 3.0)
enable_testing()
project(lw_std_build)

set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tests)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/" #source directory
        DESTINATION "include/lw_std" #target directory
        FILES_MATCHING #install only matched files
        PATTERN "*.hpp" #select header files
        PATTERN "build" EXCLUDE
        PATTERN "tests" EXCLUDE
        PATTERN ".*" EXCLUDE
        PATTERN "**CMakeFiles**" EXCLUDE)

