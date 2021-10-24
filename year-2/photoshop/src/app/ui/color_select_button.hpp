#pragma once

#include "../../ui/ui_button.hpp"
#include "../assets.hpp"

class ColorSelectButton : public UIButton {
    UIView* inner_background_view;
    UIView* inner_color_view;
    UIFillStyleTexture inner_background_view_style {};
    UIFillStyleColor inner_color_view_style {{0, 0, 0, 0}};
public:
    explicit ColorSelectButton(const Vec2f& position = {0, 0}): UIButton(position) {
        inner_background_view = new UIView({9, 9}, {32, 32});
        inner_background_view_style.set_texture(Assets.canvas_background_texture);
        inner_background_view->set_fill_style(&inner_background_view_style);

        inner_color_view = new UIView({}, {32, 32});
        inner_color_view->set_fill_style(&inner_color_view_style);

        inner_background_view->append_child(inner_color_view);
        append_child(inner_background_view);
    }

    void set_presented_color(const Vec4f& color) {
        inner_color_view_style.set_color(color);
        inner_color_view->set_needs_redraw();
    }
};