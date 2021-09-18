
#include "ui_slider.hpp"

void UISlider::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(clicked) {
        set_button_position(event->x - mouse_click_point);
    }
}

UISlider::UISlider(const Vec2d& position, const Vec2d& size): UIView(position, {size.x, BUTTON_HEIGHT}) {
    bar.set_background({0.3, 0.3, 0.3, 1});
    button.set_background({0.2, 0.2, 1, 1});
    set_background({0, 0, 0, 0});
    button.set_size({BUTTON_WIDTH, BUTTON_HEIGHT});
    append_child(&bar);
    append_child(&button);
    layout();
    locate_button();
}

void UISlider::set_fraction(double new_fraction) {
    fraction = new_fraction;
    if(fraction < 0) fraction = 0;
    if(fraction > 1) fraction = 1;

    locate_button();
    callback(fraction);
}
