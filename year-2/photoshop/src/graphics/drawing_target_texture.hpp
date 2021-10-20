#pragma once

#include "drawing_target.hpp"
#include "../utils/vec2.hpp"
#include "drawable.hpp"

class DrawingTargetTexture : public DrawingTarget, public Drawable {

    Vec2i size {};
public:
    DrawingTargetTexture(const Vec2i& texture_size) {
        size = texture_size;
        target = new sf::RenderTexture();
        ((sf::RenderTexture*)target)->create(size[0], size[1]);
        ((sf::RenderTexture*)target)->clear({0, 0, 0, 0});
    }

    const sf::Texture* get_texture() { return &((sf::RenderTexture*)target)->getTexture(); }

    Vec2i get_size() { return size; }
};