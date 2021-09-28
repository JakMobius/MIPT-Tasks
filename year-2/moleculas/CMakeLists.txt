cmake_minimum_required(VERSION 3.20)
project(moleculas)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(moleculas
        main.cpp
        physics-engine/world/world.cpp
        physics-engine/world/world.hpp
        game/creatures/game_object.cpp
        game/creatures/game_object.hpp
        game/creatures/circle.cpp
        game/creatures/circle.hpp
        game/interface_view.cpp
        game/interface_view.hpp
        physics-engine/bodies/circle_body.cpp
        physics-engine/bodies/circle_body.hpp
        physics-engine/bodies/body.cpp
        physics-engine/bodies/body.hpp
        utils/matrix3.hpp
        utils/matrix4.hpp
        utils/vec3.hpp
        utils/vec2.hpp
        utils/dynamic_array.cpp
        utils/dynamic_array.hpp
        visual-engine/user_controller.cpp
        visual-engine/user_controller.hpp
        visual-engine/drawing_context.hpp
        visual-engine/drawing_context.cpp
        visual-engine/ui/ui_text.cpp
        visual-engine/ui/ui_text.hpp
        visual-engine/ui/ui_button.cpp
        visual-engine/ui/ui_button.hpp
        visual-engine/ui/ui_slider.cpp
        visual-engine/ui/ui_slider.hpp
        visual-engine/ui/ui_view.cpp
        visual-engine/ui/ui_view.hpp
        visual-engine/events/mouse_events.cpp
        visual-engine/events/mouse_events.hpp
        physics-engine/bodies/plane_body.cpp
        physics-engine/bodies/plane_body.hpp
        physics-engine/contacts/contact.cpp
        physics-engine/contacts/contact.hpp
        physics-engine/contacts/circle_circle_contact.cpp
        physics-engine/contacts/circle_circle_contact.hpp
        physics-engine/contacts/circle_plane_contact.cpp
        physics-engine/contacts/circle_plane_contact.hpp
        game/creatures/plane.cpp
        game/creatures/plane.hpp
        game/creatures/super_circle.cpp
        game/creatures/super_circle.hpp
        game/scene.cpp game/scene.hpp
        components/component.cpp
        components/component.hpp
        visual-engine/visual_component.cpp
        visual-engine/visual_component.hpp
        components/entity.cpp
        components/entity.hpp)

target_link_libraries(moleculas sfml-graphics sfml-audio)