#pragma once

#include "../../ui/ui_button.hpp"
#include "../assets.hpp"
#include "../../ui/styles/fill_style/fill_style_texture.hpp"

class ColorSelectButton : public UIButton {
    UIView* inner_background_view = nullptr;
    UIView* inner_color_view = nullptr;
    UIFillStyleTexture inner_background_view_style {};
    UIFillStyleColor inner_color_view_style {{0, 0, 0, 0}};
public:
    explicit ColorSelectButton(const Vec2f& position = {0, 0});

    void set_presented_color(const Vec4f& color) {
        inner_color_view_style.set_color(color);
        inner_color_view->set_needs_redraw();
    }
};