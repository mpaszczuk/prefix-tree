cmake_minimum_required(VERSION 3.19)
project(rekrutacja C)
enable_testing()
add_subdirectory(lib/Unity)
set(CMAKE_C_STANDARD 11)

add_library(rekrutacja_lib STATIC
        src/trie.c
        src/ip.c
        src/mask.c
)
target_include_directories(rekrutacja_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(rekrutacja main.c )
target_link_libraries(rekrutacja rekrutacja_lib)

add_library(test_lib STATIC test/test_utils.c )
target_include_directories(test_lib PUBLIC test/include)
target_link_libraries(test_lib rekrutacja_lib)
add_executable(test_trie test/test_trie.c )
add_executable(test_api test/test_api.c )

target_link_libraries(test_trie rekrutacja_lib test_lib unity)
target_link_libraries(test_api rekrutacja_lib test_lib unity)

add_test(test_trie test_trie)
add_test(test_api test_api)
