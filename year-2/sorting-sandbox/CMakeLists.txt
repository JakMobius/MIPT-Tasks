cmake_minimum_required(VERSION 3.20)
project(sorting_sandbox)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER clang++)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(sorting_sandbox ${files})

#set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")

target_link_libraries(sorting_sandbox sfml-graphics sfml-audio)