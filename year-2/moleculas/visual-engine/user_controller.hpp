#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ui/ui_view.hpp"

class UserController {
    sf::RenderWindow* window;
    UIView* root_view;

    int old_mouse_x = -1;
    int old_mouse_y = -1;
public:

    explicit UserController(sf::RenderWindow* window, UIView* root_view): window(window), root_view(root_view) {};

    void handle_event(const sf::Event &event);
    void tick();

};