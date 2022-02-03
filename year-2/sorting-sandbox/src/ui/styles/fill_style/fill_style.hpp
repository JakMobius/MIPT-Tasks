#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include "../../../utils/vec2.hpp"
#include "../../../utils/vec4.hpp"
#include "../../../graphics/drawing-target-texture.hpp"

class UIAnimatedFillStyle;

enum class UIFillStyleType {
    invalid, color, texture, gradient, animated
};

class UIFillStyle {
protected:
    UIFillStyleType type = UIFillStyleType::invalid;
    sf::RenderStates render_states {};
public:
    virtual ~UIFillStyle() {};
    virtual const sf::RenderStates* get_render_states() const { return &render_states; };
    virtual sf::RenderStates* get_render_states() { return &render_states; };
    virtual sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const = 0;

    virtual UIAnimatedFillStyle* get_animated(const UIFillStyle* other) const { return nullptr; };

    UIFillStyleType get_type() const { return type; }
};

class UIAnimatedFillStyle : public UIFillStyle {
protected:
    float fraction = 0;
public:
    virtual void set_animation_fraction(float p_fraction) { fraction = p_fraction; };
};