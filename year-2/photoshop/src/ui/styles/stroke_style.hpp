#pragma once

#include <SFML/Graphics.hpp>
#include "../../utils/vec2.hpp"
#include "../../utils/vec4.hpp"

class UIStrokeStyle {
protected:
    sf::RenderStates render_states {};
public:
    virtual const sf::RenderStates* get_render_states() const { return &render_states; };
    virtual sf::RenderStates* get_render_states() { return &render_states; };
    virtual sf::Vertex vertex(Vec2f vertex_position, float stroke_depth, float stroke_offset) const = 0;
};

class UIStrokeStyleColor : public UIStrokeStyle {
    sf::Color color;
public:
    explicit UIStrokeStyleColor(const Vec4f& color): color(color.to_sf_color()) {}

    sf::Vertex vertex(Vec2f vertex_position, float stroke_depth, float stroke_offset) const override {
        return {{vertex_position[0], vertex_position[1]}, color};
    };

    void set_color(const Vec4f& p_color) { color = p_color.to_sf_color(); }
};