cmake_minimum_required(VERSION 3.1)
enable_testing()

if(NOT TARGET lw_std_test_suite)
    project(lw_std_test_suite)

    add_executable(lw_std_test_suite
        testing.cpp
        container_tester/container_tester_helpers.cpp
        compile_accelerators/accelerate_vector.cpp
        compile_accelerators/accelerate_list.cpp
        compile_accelerators/accelerate_unordered_map.cpp
        compile_accelerators/accelerate_unordered_set.cpp
        compile_accelerators/accelerate_queue.cpp
    )

    target_compile_definitions(lw_std_test_suite PRIVATE LWSTD_BUILD_STD_COMPATIBILITY)

    target_include_directories(lw_std_test_suite PRIVATE ../src/)
    target_include_directories(lw_std_test_suite PRIVATE ../dependencies/)

    target_link_libraries(lw_std_test_suite PRIVATE ftest)

    target_compile_options(lw_std_test_suite PRIVATE -std=c++17 -pedantic
        -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Wconversion -Wsign-conversion -Wmisleading-indentation
        -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wextra-semi)

    if(NOT DEFINED DISABLE_ASAN)
        target_compile_options(lw_std_test_suite PRIVATE -fsanitize=address)
        target_link_options(lw_std_test_suite PRIVATE -lasan)
    else()
        message("WARNING: address sanitizer has been disabled, memory access errors may not be detected")
    endif()

    add_test(lw_std_test_suite lw_std_test_suite)
endif()