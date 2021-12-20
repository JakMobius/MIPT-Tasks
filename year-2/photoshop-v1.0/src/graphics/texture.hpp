#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "../utils/vec2.hpp"
#include "textured.hpp"

class Texture : public Textured {

    sf::Texture* texture;
    Vec2i size {};
public:
    explicit Texture(const Vec2i& texture_size);
    ~Texture();

    explicit Texture(const char* path);
    explicit Texture(const Vec2i size, const unsigned char* rgba_array);

    void set_repeating(bool is_repeating);
    const sf::Texture* get_texture() override { return texture; }

    Vec2i get_size() { return size; }
};