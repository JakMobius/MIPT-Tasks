#pragma once

#include "drawing_target.hpp"
#include "../utils/vec2.hpp"
#include "textured.hpp"
#include "../utils/vec4.hpp"

class DrawingTargetTexture : public DrawingTarget, public Textured {

    Vec2i size {};
public:
    explicit DrawingTargetTexture(const Vec2i& texture_size): DrawingTarget() {
        size = texture_size;
        target = new sf::RenderTexture();
        ((sf::RenderTexture*)target)->create(size[0], size[1]);
    }

    DrawingTargetTexture(const DrawingTargetTexture& copy): DrawingTarget(copy) {
        size = copy.size;
        sf::RenderTexture* target = new sf::RenderTexture();
        target->create(copy.size[0], copy.size[1]);

        sf::Sprite sprite;
        sprite.setTexture(*copy.get_texture(), true);
        target->draw(sprite);
        this->target = target;
    }

    ~DrawingTargetTexture() { delete target; }

    void clear(const Vec4f& color) {
        ((sf::RenderTexture*)target)->clear(color.to_sf_color());
    }

    sf::RenderTexture* get_render_texture() { return (sf::RenderTexture*)target; }
    const sf::Texture* get_texture() const { return &((sf::RenderTexture*)target)->getTexture(); }

    Vec2i get_size() { return size; }
};