
#include "ui_slider.hpp"

void UISlider::on_mouse_move(MouseMoveEvent* event) {
    UIView::on_mouse_move(event);
    if(clicked) {
        set_button_position(event->x - mouse_click_point);
    }
}

UISlider::UISlider(const Vec2f& position, const Vec2f& size): UIView(position, size) {
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
        float half_buffon_width = button->get_size()[0] / 2;
        set_button_position(event->x - half_buffon_width);
        mouse_click_point = half_buffon_width;
    } else {
        mouse_click_point = event->x - button->get_position()[0];
    }
}

void UISlider::set_button_position(float x) {
    float available_width = size[0] - button->get_size()[0];

    Vec2f button_position = button->get_position();
    button_position.set_x(x);
    if(button_position[0] < 0) button_position.set_x(0);
    if(button_position[0] > available_width) button_position.set_x(available_width);
    button->set_position(button_position);

    fraction = button_position[0] / available_width;
    if(callback) callback(fraction);
}

void UISlider::layout() {
    UIView::layout();
    if(!style) set_style(UISliderStyle::instance);
    bar->set_size({size[0], bar->get_size()[1]});
    bar->set_position({0, button->get_size()[1] / 2 - style->get_bar_height() / 2});
    float position_x = (size[0] - button->get_size()[0]) * fraction;
    button->set_position({position_x, 0});
    set_size(size);
}

void UISlider::set_style(const UISliderStyle* p_style) {
    Styled::set_style(p_style);
    bar->set_fill_style(style->get_bar_color());
    button->set_fill_style(style->get_button_color());
    button->set_size(style->get_button_size());
    set_size(size);
    bar->set_size({size[0], style->get_bar_height()});
}

void UISlider::set_size(const Vec2f &new_size) {
    Vec2f real_size = {new_size[0], button->get_size()[1]};
    UIView::set_size(real_size);
}
