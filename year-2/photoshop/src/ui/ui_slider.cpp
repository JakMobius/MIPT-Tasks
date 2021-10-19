
#include "ui_slider.hpp"

void UISlider::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(clicked) {
        set_button_position(event->x - mouse_click_point);
    }
}

UISlider::UISlider(const Vec2f& position, const Vec2f& size): UIView(position, {size[0], BUTTON_HEIGHT}) {
    bar->set_background({0.3, 0.3, 0.3, 1});
    button->set_background({0.2, 0.2, 1, 1});
    button->set_size({BUTTON_WIDTH, BUTTON_HEIGHT});
    append_child(bar);
    append_child(button);
}

void UISlider::set_fraction(float new_fraction) {
    fraction = new_fraction;
    if(fraction < 0) fraction = 0;
    if(fraction > 1) fraction = 1;

    set_needs_layout();
    if(callback) callback(fraction);
}