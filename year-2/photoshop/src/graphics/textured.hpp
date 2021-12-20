#pragma once

#include <SFML/Graphics/Texture.hpp>

class Textured {
public:
    virtual const sf::Texture* get_texture() const = 0;
};