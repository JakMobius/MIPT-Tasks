#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "../utils/vec2.hpp"
#include "drawable.hpp"

class DrawableTexture : public Drawable {

    sf::Texture* texture;
    Vec2i size {};
public:
    DrawableTexture(const Vec2i& texture_size) {
        size = texture_size;
        texture = new sf::Texture();
        texture->create(size[0], size[1]);
    }

    DrawableTexture(const char* path) {
        texture = new sf::Texture();
        texture->loadFromFile(path);
    }

    const sf::Texture* get_texture() override { return texture; }

    Vec2i get_size() { return size; }
};