#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include "../../utils/vec2.hpp"
#include "../../utils/vec4.hpp"
#include "../../graphics/drawing_target_texture.hpp"

class UIFillStyle {
protected:
    sf::RenderStates render_states {};
public:
    virtual const sf::RenderStates* get_render_states() const { return &render_states; };
    virtual sf::RenderStates* get_render_states() { return &render_states; };
    virtual sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const = 0;
};

class UIFillStyleColor : public UIFillStyle {
protected:
    sf::Color color;
public:
    explicit UIFillStyleColor(const Vec4f& color): color(color.to_sf_color()) {}

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {{vertex_position[0], vertex_position[1]}, color};
    };

    void set_color(const Vec4f& p_color) { color = p_color.to_sf_color(); }
};

class UIFillStyleTexture : public UIFillStyleColor {
    Vec2f scale { 1, 1 };
    Textured* texture;
public:
    explicit UIFillStyleTexture(Textured* texture): UIFillStyleColor({1, 1, 1, 1}), texture(texture) { render_states.texture = texture->get_texture(); }
    explicit UIFillStyleTexture(): UIFillStyleColor({1, 1, 1, 1}) {}

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

    void set_scale(const Vec2f& p_scale) { scale = p_scale; }

    Textured* get_texture() { return texture; }
};

struct GradientAnchorPoint {
    Vec4f color;
    Vec2f position;
};

class UIFillStyleGradient : public UIFillStyle {
    GradientAnchorPoint start_anchor;
    GradientAnchorPoint end_anchor;

public:
    UIFillStyleGradient() {}

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override;;

    GradientAnchorPoint* get_start_anchor() { return &start_anchor; }
    void set_start_anchor(const GradientAnchorPoint &p_start_anchor) { start_anchor = p_start_anchor; }
    GradientAnchorPoint* get_end_anchor() { return &end_anchor; }
    void set_end_anchor(const GradientAnchorPoint &p_end_anchor) { end_anchor = p_end_anchor; }
};