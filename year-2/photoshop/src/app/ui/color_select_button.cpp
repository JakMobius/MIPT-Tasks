//
// Created by Артем on 24.10.2021.
//

#include "color_select_button.hpp"

ColorSelectButton::ColorSelectButton(const Vec2f &position) : UIButton(position) {
    inner_background_view = new UIView({9, 9}, {32, 32});
    inner_background_view_style.set_texture(Assets.canvas_background_texture);
    inner_background_view->set_fill_style(&inner_background_view_style);

    inner_color_view = new UIView({}, {32, 32});
    inner_color_view->set_fill_style(&inner_color_view_style);

    inner_background_view->append_child(inner_color_view);
    append_child(inner_background_view);
}
