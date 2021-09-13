cmake_minimum_required(VERSION 3.20)
project(ray_casting)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -O3)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(ray_casting main.cpp sphere.cpp sphere.hpp vec2.cpp vec2.hpp vec3.cpp vec3.hpp camera.cpp camera.hpp world.cpp world.hpp light.cpp light.hpp ray.cpp ray.hpp user_controller.cpp user_controller.hpp matrix4.cpp matrix4.hpp renderer.cpp renderer.hpp semaphores.h user_viewport.cpp user_viewport.hpp)
target_link_libraries(ray_casting sfml-graphics sfml-audio)
