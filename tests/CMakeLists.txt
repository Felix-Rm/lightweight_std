cmake_minimum_required(VERSION 3.0)
enable_testing()


if(NOT TARGET lw_std_test_suite)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -Wall -fsanitize=address")

add_subdirectory(ftest)

project(lw_std_test_suite)

add_executable(lw_std_test_suite testing.cpp)

target_include_directories(lw_std_test_suite PRIVATE ../)
target_link_libraries(lw_std_test_suite PRIVATE ftest)

add_test(lw_std_test_suite lw_std_test_suite)

endif()