#pragma once

#include "fill_style/fill_style.hpp"
#include "fill_style/fill_style_color.hpp"

struct UISliderStyle {
    UISliderStyle() = default;
    virtual ~UISliderStyle() = default;

    static const UISliderStyle* instance;
    virtual const UIFillStyle* get_button_color()  const { static UIFillStyleColor color({0.2, 0.2, 1.0, 1.0}); return &color; };
    virtual const UIFillStyle* get_bar_color()     const { static UIFillStyleColor color({0.3, 0.3, 0.3, 1.0}); return &color; };

    virtual float get_bar_height()  const { return 10; }
    virtual Vec2f get_button_size() const { return {40, 40}; }
};