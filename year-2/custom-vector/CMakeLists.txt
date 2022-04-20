cmake_minimum_required(VERSION 3.20)
project(custom-vector)

set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(custom-vector ${files})