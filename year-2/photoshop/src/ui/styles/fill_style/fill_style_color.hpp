#pragma once

#include "fill_style.hpp"

class UIAnimatedFillStyleColor : public UIAnimatedFillStyle {
    Vec4f color_a;
    Vec4f color_b;
    Vec4f current_color;
public:
    UIAnimatedFillStyleColor(const Vec4f& color_a, const Vec4f& color_b): color_a(color_a), color_b(color_b) {
        current_color = color_a;
    }

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {{vertex_position[0], vertex_position[1]}, get_current_animation_color().to_sf_color()};
    };

    void set_animation_fraction(float p_fraction) override {
        UIAnimatedFillStyle::set_animation_fraction(p_fraction);
        current_color = color_a * (1 - fraction) += color_b * fraction;
    }

    UIAnimatedFillStyle* get_animated(const UIFillStyle* other) const override;

    const Vec4f& get_current_animation_color() const { return current_color; }
};

class UIFillStyleColor : public UIFillStyle {
protected:
    sf::Color color;
public:
    explicit UIFillStyleColor(const Vec4f& color): color(color.to_sf_color()) {
        type = UIFillStyleType::color;
    }

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override {
        return {{vertex_position[0], vertex_position[1]}, color};
    };

    void set_color(const Vec4f& p_color) { color = p_color.to_sf_color(); }
    Vec4f get_color() const { return Vec4f::from_sf_color(color); }

    UIAnimatedFillStyle* get_animated(const UIFillStyle* other) const override;
};