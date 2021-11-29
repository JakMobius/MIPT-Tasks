#pragma once

#include "../../../utils/vec4.hpp"
#include "../../../utils/vec2.hpp"
#include "fill_style.hpp"

struct GradientAnchorPoint {
    Vec4f color;
    Vec2f position;
};

class UIFillStyleGradient : public UIFillStyle {
    GradientAnchorPoint start_anchor;
    GradientAnchorPoint end_anchor;

public:
    UIFillStyleGradient() {
        type = UIFillStyleType::gradient;
    }

    sf::Vertex vertex(Vec2f vertex_position, Vec2f shape_position) const override;;

    GradientAnchorPoint* get_start_anchor() { return &start_anchor; }
    void set_start_anchor(const GradientAnchorPoint &p_start_anchor) { start_anchor = p_start_anchor; }
    GradientAnchorPoint* get_end_anchor() { return &end_anchor; }
    void set_end_anchor(const GradientAnchorPoint &p_end_anchor) { end_anchor = p_end_anchor; }

    UIAnimatedFillStyle* get_animated(const UIFillStyle* other) const override { return nullptr; }
};