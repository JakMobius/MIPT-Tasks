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
    sf::Color color;
public:
    explicit UIFillStyleColor(const Vec4f& color): color(color.to_sf_color()) {}

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {{vertex_position[0], vertex_position[1]}, color};
    };

    void set_color(const Vec4f& p_color) { color = p_color.to_sf_color(); }
};

class UIFillStyleTexture : public UIFillStyle {
    Vec2f scale { 1, 1 };
public:
    explicit UIFillStyleTexture(Drawable* texture) { render_states.texture = texture->get_texture(); }
    explicit UIFillStyleTexture() {}

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {{vertex_position[0], vertex_position[1]}, {shape_position[0] * scale[0], shape_position[1] * scale[1]}};
    };

    void set_texture(Drawable* texture) { render_states.texture = texture->get_texture(); }

    void set_scale(const Vec2f& p_scale) { scale = p_scale; }
};