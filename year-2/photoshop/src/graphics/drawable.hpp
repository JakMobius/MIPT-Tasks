#pragma once

#include <SFML/Graphics/Texture.hpp>

class Drawable {
public:
    virtual const sf::Texture* get_texture() = 0;
};