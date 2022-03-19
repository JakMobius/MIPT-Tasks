cmake_minimum_required(VERSION 3.20)
project(copy_elisioner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(copy_elisioner ${files})

#set(CMAKE_CXX_FLAGS "-O0")
#set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address -fno-elide-constructors -DALLOW_SMARTVALUE_MOVE")
set(CMAKE_CXX_FLAGS "-O3 -fsanitize=address -DALLOW_SMARTVALUE_MOVE")
#set(CMAKE_CXX_FLAGS "-O3 -fsanitize=address")

target_link_libraries(copy_elisioner sfml-graphics sfml-audio)
