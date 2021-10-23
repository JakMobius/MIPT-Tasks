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

    void set_texture(Drawable* texture) {
        if(texture) {
            render_states.texture = texture->get_texture();
        } else {
            render_states.texture = nullptr;
        }
    }

    void set_scale(const Vec2f& p_scale) { scale = p_scale; }
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

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        Vec2f relative_position { shape_position };
        relative_position -= start_anchor.position;

        Vec2f gradient_vector = end_anchor.position;
        gradient_vector -= start_anchor.position;

        float fraction = relative_position.dot(gradient_vector) / gradient_vector.len_squared();
        if(fraction < 0) fraction = 0;
        else if(fraction > 1) fraction = 1;

        Vec4f end_color { end_anchor.color };
        end_color *= fraction;

        Vec4f start_color { start_anchor.color };
        start_color *= (1 - fraction);

        start_color += end_color;

        return {{vertex_position[0], vertex_position[1]}, start_color.to_sf_color()};
    };

    GradientAnchorPoint* get_start_anchor() { return &start_anchor; }
    void set_start_anchor(const GradientAnchorPoint &p_start_anchor) { start_anchor = p_start_anchor; }
    GradientAnchorPoint* get_end_anchor() { return &end_anchor; }
    void set_end_anchor(const GradientAnchorPoint &p_end_anchor) { end_anchor = p_end_anchor; }
};