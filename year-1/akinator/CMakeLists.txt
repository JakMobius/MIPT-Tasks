cmake_minimum_required(VERSION 3.17)
project(akinator C)

set(CMAKE_C_STANDARD 11)

add_executable(akinator main.c file_io.c file_io.h tree.c tree.h tree_parser.c akinator.c akinator.h akinator_image.c akinator_image.h tree_searcher.c)