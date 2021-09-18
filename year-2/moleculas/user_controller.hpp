#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class UserController {
    sf::RenderWindow* window;
public:

    explicit UserController(sf::RenderWindow* window): window(window) {};

    void handle_event(const sf::Event &event);
    void tick();

};