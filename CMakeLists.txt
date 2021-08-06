cmake_minimum_required(VERSION 3.19)
project(rekrutacja C)

set(CMAKE_C_STANDARD 11)
set(SRC
    src/tree.c
)

add_executable(${CMAKE_PROJECT_NAME}
        main.c
        ${SRC}
        )

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)