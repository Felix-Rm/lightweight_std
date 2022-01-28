cmake_minimum_required(VERSION 3.0)
enable_testing()

set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=17")


add_subdirectory(tests)


install(DIRECTORY "${CMAKE_SOURCE_DIR}/" #source directory
        DESTINATION "include/lw_std" #target directory
        FILES_MATCHING #install only matched files
        PATTERN "*.hpp" #select header files
        PATTERN "build" EXCLUDE
        PATTERN "tests" EXCLUDE
        PATTERN ".*" EXCLUDE
        PATTERN "**CMakeFiles**" EXCLUDE)

