cmake_minimum_required(VERSION 3.17)
project(Mandelbrot C)

set(CMAKE_C_FLAGS -march=native)
set(CMAKE_C_FLAGS -fsanitize=address)
set(CMAKE_C_FLAGS -O3)
set(CMAKE_C_STANDARD 11)
set(SDL2_INCLUDE_DIRS "/Library/Frameworks/SDL2.framework/Headers")

find_library(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

include_directories(Mandelbrot ${SDL2_INCLUDE_DIRS})
include_directories(Mandelbrot ${OPENGL_INCLUDE_DIRS})

add_executable(Mandelbrot main.c videomemory.c bmp_reader.c color_blend.c game.c cat_controller.c mandelbrot_controller.c mandelbrot_methods/draw_nosse.c mandelbrot_methods/draw_pd_128_messed.c mandelbrot_methods/draw_pd_128.c mandelbrot_methods/draw_ps_128.c messed_drawer.c messed_drawer.h mandelbrot_thread.c mandelbrot_thread.h semaphores.h)

target_link_libraries(Mandelbrot ${SDL2_LIBRARIES})
target_link_libraries(Mandelbrot ${OPENGL_LIBRARIES})
target_link_libraries(Mandelbrot ${PNG_LIBRARY})

if (APPLE)
    target_link_libraries(Mandelbrot "-framework OpenGL")
endif()

file(COPY resources DESTINATION .)