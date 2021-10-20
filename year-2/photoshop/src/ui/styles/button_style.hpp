#pragma once

#include "fill_style.hpp"

struct UIButtonStyle {
    UIButtonStyle() = default;
    virtual ~UIButtonStyle() = default;

    static const UIButtonStyle* instance;
    virtual const UIFillStyle* get_hovered_color()  const { static UIFillStyleColor color({0.0, 1.0, 0.0, 1.0}); return &color; };
    virtual const UIFillStyle* get_idle_color()     const { static UIFillStyleColor color({0.8, 0.8, 0.8, 1.0}); return &color; };
    virtual const UIFillStyle* get_clicked_color()  const { static UIFillStyleColor color({0.0, 0.8, 0.0, 1.0}); return &color; };
    virtual const UIFillStyle* get_selected_color() const { static UIFillStyleColor color({0.6, 0.6, 0.8, 1.0}); return &color; };
    virtual const UIFillStyle* get_disabled_color() const { static UIFillStyleColor color({0.4, 0.4, 0.4, 1.0}); return &color; };
    virtual const UIFillStyle* get_inactive_color() const { static UIFillStyleColor color({0.6, 0.6, 0.6, 1.0}); return &color; };
};