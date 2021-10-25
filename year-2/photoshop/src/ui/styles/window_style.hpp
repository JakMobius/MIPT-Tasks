#pragma once

#include "../../utils/vec4.hpp"
#include "./button_style.hpp"
#include "../../app/assets.hpp"

struct UIDefaultCloseButtonStyle : public UIButtonStyle {
    static UIDefaultCloseButtonStyle* instance;

    const UIFillStyle* get_hovered_color() const override {
        static UIFillStyleTexture texture {};
        if(!texture.get_texture()) texture.set_texture(Assets.window_close_hover_icon);
        return &texture;
    }
    const UIFillStyle* get_inactive_color()    const override {
        static UIFillStyleTexture texture {};
        if(!texture.get_texture()) texture.set_texture(Assets.window_close_inactive_icon);
        return &texture;
    }
    const UIFillStyle* get_idle_color()    const override {
        static UIFillStyleTexture texture {};
        if(!texture.get_texture()) texture.set_texture(Assets.window_close_icon);
        return &texture;
    }
    const UIFillStyle* get_clicked_color() const override {
        static UIFillStyleTexture texture {};
        if(!texture.get_texture()) texture.set_texture(Assets.window_close_clicked_icon);
        return &texture;
    }
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
    virtual const Vec4f          get_window_header_color()              const { return Vec4f {0.702,  0.69,  0.694,  1}; };
    virtual const UIFillStyle*   get_window_background_color()          const { static UIFillStyleColor color({0.153,  0.125, 0.141,  1}); return &color; };
    virtual const UIFillStyle*   get_header_background_color()          const { static UIFillStyleColor color({0.30,  0.30,  0.3,   1}); return &color; };
    virtual const UIFillStyle*   get_inactive_header_background_color() const { static UIFillStyleColor color({0.235, 0.247, 0.254, 1}); return &color; };
};