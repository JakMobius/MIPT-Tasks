#pragma once

class Drawable {
public:
    virtual const sf::Texture* get_texture() = 0;
};