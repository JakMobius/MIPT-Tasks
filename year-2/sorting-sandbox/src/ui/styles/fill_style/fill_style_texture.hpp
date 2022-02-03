#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include "../../../utils/vec2.hpp"
#include "../../../graphics/textured.hpp"
#include "fill_style_color.hpp"

class UIFillStyleTexture : public UIFillStyleColor {
    Vec2f scale { 1, 1 };
    Textured* texture;
public:
    explicit UIFillStyleTexture(Textured* texture): UIFillStyleColor({1, 1, 1, 1}), texture(texture) {
        if(texture) {
            render_states.texture = texture->get_texture();
        }
        type = UIFillStyleType::texture;
    }
    explicit UIFillStyleTexture(): UIFillStyleColor({1, 1, 1, 1}) {
        type = UIFillStyleType::texture;
    }

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {
            {vertex_position[0], vertex_position[1]},
            color,
            {shape_position[0] * scale[0], shape_position[1] * scale[1]}
        };
    };

    void set_texture(Textured* p_texture) {
        texture = p_texture;
        if(texture) {
            render_states.texture = texture->get_texture();
        } else {
            render_states.texture = nullptr;
        }
    }

    UIAnimatedFillStyle* get_animated(const UIFillStyle* other) const override { return nullptr; }

    void set_scale(const Vec2f& p_scale) { scale = p_scale; }

    Textured* get_texture() { return texture; }
};