cmake_minimum_required(VERSION 3.20)
project(moleculas)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(moleculas
        main.cpp
        world.cpp
        world.hpp
        creatures/creature.cpp
        creatures/creature.hpp
        creatures/circle.cpp
        creatures/circle.hpp
        fixtures/circle_fixture.cpp
        fixtures/circle_fixture.hpp
        fixtures/fixture.cpp
        fixtures/fixture.hpp
        utils/matrix3.hpp
        utils/matrix4.hpp
        utils/vec3.hpp
        utils/vec2.hpp
        utils/dynamic_array.cpp
        utils/dynamic_array.hpp
        user_controller.cpp
        user_controller.hpp
        drawing_context.hpp
        drawing_context.cpp fixtures/plane_fixture.cpp fixtures/plane_fixture.hpp contacts/contact.cpp contacts/contact.hpp contacts/circle_circle_contact.cpp contacts/circle_circle_contact.hpp contacts/circle_plane_contact.cpp contacts/circle_plane_contact.hpp creatures/plane.cpp creatures/plane.hpp)

target_link_libraries(moleculas sfml-graphics sfml-audio)