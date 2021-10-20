
#include "ui_slider.hpp"

void UISlider::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(clicked) {
        set_button_position(event->x - mouse_click_point);
    }
}

UISlider::UISlider(const Vec2f& position, const Vec2f& size): UIView(position, {size[0], BUTTON_HEIGHT}) {
    set_style(UISliderStyle::instance);
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

void UISlider::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);
    if(current_clicked_child != button) {
        set_button_position(event->x - BUTTON_WIDTH / 2);
        mouse_click_point = BUTTON_WIDTH / 2;
    } else {
        mouse_click_point = event->x - button->get_position()[0];
    }
}

void UISlider::set_button_position(float x) {
    float max_right = size[0] - BUTTON_WIDTH;

    Vec2f button_position = button->get_position();
    button_position.set_x(x);
    if(button_position[0] < 0) button_position.set_x(0);
    if(button_position[0] > max_right) button_position.set_x(max_right);
    button->set_position(button_position);

    fraction = button_position[0] / (size[0] - BUTTON_WIDTH);
    if(callback) callback(fraction);
}

void UISlider::layout() {
    UIView::layout();
    bar->set_position({0, BUTTON_HEIGHT / 2 - BAR_HEIGHT / 2});
    bar->set_size({size[0], BAR_HEIGHT});
    float position_x = (size[0] - BUTTON_WIDTH) * fraction;
    button->set_position({position_x, 0});
}

void UISlider::set_style(const UISliderStyle* p_style) {
    style = p_style;
    bar->set_fill_style(style->get_bar_color());
    button->set_fill_style(style->get_bar_color());
}
