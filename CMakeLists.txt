cmake_minimum_required(VERSION 3.19)
project(rekrutacja C)
enable_testing()
add_subdirectory(lib/Unity)

set(CMAKE_C_STANDARD 11)

add_library(rekrutacja_lib STATIC
        src/trie.c
        src/ip.c
)

target_include_directories(rekrutacja_lib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

add_executable(rekrutacja main.c )
add_executable(tree_test
        test/test.c
        test/utils.c
        )

target_include_directories(tree_test PUBLIC
        ${CMAKE_SOURCE_DIR}/test/include
        )

target_link_libraries(rekrutacja rekrutacja_lib)
target_link_libraries(tree_test rekrutacja_lib unity)

add_test(tree_test tree_test)
