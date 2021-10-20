#pragma once

#include "fill_style.hpp"
#include "button_style.hpp"

struct UISelectViewStyle {
    UISelectViewStyle() = default;
    virtual ~UISelectViewStyle() = default;

    static const UISelectViewStyle* instance;
    virtual const UIFillStyle* get_background_color() const { static UIFillStyleColor color({0.7, 0.7, 0.7, 1.0}); return &color; };
    virtual const UIButtonStyle* get_button_style()   const { return UIButtonStyle::instance; };
};