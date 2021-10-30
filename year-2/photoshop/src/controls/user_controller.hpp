#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../ui/ui_view.hpp"

class UserController {
    sf::Window* window;
    UIView* root_view;
public:

    explicit UserController(sf::Window* window, UIView* root_view): window(window), root_view(root_view) {};

    void handle_event(const sf::Event &event);

};