#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "camera.hpp"

class UserController {
    bool w_pressed = false;
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;
    bool shift_pressed = false;
    bool space_pressed = false;
    bool up_pressed = false;
    bool right_pressed = false;
    bool left_pressed = false;
    bool down_pressed = false;
    Camera* controlled_camera;
    sf::RenderWindow* window;
public:

    explicit UserController(Camera* camera, sf::RenderWindow* window): controlled_camera(camera), window(window) {};

    void handle_event(const sf::Event &event);
    void tick();

};