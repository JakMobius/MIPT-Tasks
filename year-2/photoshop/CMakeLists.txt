cmake_minimum_required(VERSION 3.20)
project(photoshop)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")

message(${files})
add_executable(photoshop ${files})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -O0)

target_link_libraries(photoshop sfml-graphics sfml-audio)