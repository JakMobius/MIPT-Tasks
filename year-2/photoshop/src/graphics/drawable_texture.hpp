#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "../utils/vec2.hpp"
#include "drawable.hpp"

class DrawableTexture : public Drawable {

    sf::Texture* texture;
    Vec2i size {};
public:
    explicit DrawableTexture(const Vec2i& texture_size);
    ~DrawableTexture();

    explicit DrawableTexture(const char* path);
    explicit DrawableTexture(const Vec2i size, const unsigned char* rgba_array);

    void set_repeating(bool is_repeating);
    const sf::Texture* get_texture() override { return texture; }

    Vec2i get_size() { return size; }
};