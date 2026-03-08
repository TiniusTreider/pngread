cmake_minimum_required(VERSION 3.10)

project(pngread C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
add_executable(
    pngread
    src/main.c
    src/parser.c
    src/chunks.c
    src/error.c
    src/io.c
)

