#pragma once

#include "../../utils/vec4.hpp"
#include "./button_style.hpp"

struct UIDefaultCloseButtonStyle : public UIButtonStyle {
    static UIDefaultCloseButtonStyle* instance;

    const UIFillStyle* get_hovered_color() const override { static UIFillStyleColor color({1.0, 0.5, 0.5, 1}); return &color; }
    const UIFillStyle* get_idle_color()    const override { static UIFillStyleColor color({1.0, 0.3, 0.3, 1}); return &color; }
    const UIFillStyle* get_clicked_color() const override { static UIFillStyleColor color({0.6, 0.2, 0.2, 1}); return &color; }
};

struct UIDefaultFullscreenButtonStyle : public UIButtonStyle {
    static UIDefaultFullscreenButtonStyle* instance;

    const UIFillStyle* get_hovered_color() const override { static UIFillStyleColor color({0.5, 1.0, 0.5, 1}); return &color; }
    const UIFillStyle* get_idle_color()    const override { static UIFillStyleColor color({0.3, 1.0, 0.3, 1}); return &color; }
    const UIFillStyle* get_clicked_color() const override { static UIFillStyleColor color({0.2, 0.6, 0.2, 1}); return &color; }
};

struct UIWindowStyle {
    static UIWindowStyle* instance;

    virtual const UIButtonStyle* get_close_button_style()      const { return UIDefaultCloseButtonStyle::instance; };
    virtual const UIButtonStyle* get_fullscreen_button_style() const { return UIDefaultFullscreenButtonStyle::instance; };
    virtual const UIFillStyle*   get_window_background_color()          const { static UIFillStyleColor color({1,   1,   1,   1}); return &color; };
    virtual const UIFillStyle*   get_header_background_color()          const { static UIFillStyleColor color({0.3, 0.5, 0.3, 1}); return &color; };
    virtual const UIFillStyle*   get_inactive_header_background_color() const { static UIFillStyleColor color({0.3, 0.3, 0.3, 1}); return &color; };
};