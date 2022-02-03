#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../ui/ui_view.hpp"

class UserController {
    sf::Window* window;
    UIView* root_view;

    bool ctrl_pressed = false;
    bool cmd_pressed = false;
    bool shift_pressed = false;
    bool alt_pressed = false;

public:

    explicit UserController(sf::Window* window, UIView* root_view): window(window), root_view(root_view) {};

    void handle_event(const sf::Event &event);

    KeyModifiers get_modifiers();
};