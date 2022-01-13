cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -Wall")


install(DIRECTORY "${CMAKE_SOURCE_DIR}/" #source directory
        DESTINATION "include/lw_std" #target directory
        FILES_MATCHING #install only matched files
        PATTERN "*.hpp" #select header files
        PATTERN "build" EXCLUDE
        PATTERN "tests" EXCLUDE
        PATTERN ".*" EXCLUDE
        PATTERN "**CMakeFiles**" EXCLUDE)

project(lw_std_test_suite)

add_executable(lw_std_test_suite tests/testing.cpp)
target_link_libraries(lw_std_test_suite PRIVATE ftest)

add_test(test_suite lw_std_test_suite)
