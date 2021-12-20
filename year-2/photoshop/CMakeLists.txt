cmake_minimum_required(VERSION 3.20)
project(photoshop)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(photoshop ${files})

add_custom_command(TARGET photoshop PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

add_custom_command(TARGET photoshop PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/plugins/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O0")
#set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")

target_link_libraries(photoshop sfml-graphics sfml-audio)