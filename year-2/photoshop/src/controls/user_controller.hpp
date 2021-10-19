#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../ui/ui_view.hpp"

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
    sf::Window* window;
    UIView* root_view;
public:

    explicit UserController(sf::Window* window, UIView* root_view): window(window), root_view(root_view) {};

    void handle_event(const sf::Event &event);
    void tick();

};