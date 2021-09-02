cmake_minimum_required(VERSION 3.19)
project(graphics-zhiraphics)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(graphics-zhiraphics main.cpp drawing_context.cpp drawing_context.hpp matrix3.cpp matrix3.hpp vec2.cpp vec2.cpp vec2.hpp graph.cpp graph.hpp)
target_link_libraries(graphics-zhiraphics sfml-graphics sfml-audio)