cmake_minimum_required(VERSION 3.19)
project(rekrutacja C)
enable_testing()
add_subdirectory(lib/Unity)

set(CMAKE_C_STANDARD 11)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC
        src/tree.c
        src/ip.c
        )

target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} main.c )
add_executable(tree_test test/test.c)

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
target_link_libraries(tree_test ${CMAKE_PROJECT_NAME}_lib unity)

add_test(tree_test tree_test)
