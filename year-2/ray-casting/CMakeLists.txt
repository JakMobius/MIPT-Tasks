cmake_minimum_required(VERSION 3.20)
project(ray_casting)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -O1)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(ray_casting main.cpp shapes/sphere.cpp shapes/sphere.hpp utils/vec2.hpp utils/vec3.hpp camera.cpp camera.hpp world.cpp world.hpp light.cpp light.hpp ray.cpp ray.hpp user_controller.cpp user_controller.hpp utils/matrix4.hpp renderer.cpp renderer.hpp semaphores.h user_viewport.cpp user_viewport.hpp ui/ui_view.cpp ui/ui_view.hpp utils/matrix3.hpp drawing_context.cpp drawing_context.hpp utils/matrix3.hpp utils/vec4.hpp ui/ui_text.cpp ui/ui_text.hpp ui/ui_button.cpp ui/ui_button.hpp events/mouse_events.cpp events/mouse_events.hpp ui/point_test_iterator.cpp ui/point_test_iterator.hpp)
target_link_libraries(ray_casting sfml-graphics sfml-audio)
