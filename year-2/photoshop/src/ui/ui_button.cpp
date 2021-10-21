//
// Created by Артем on 13.09.2021.
//

#include "ui_button.hpp"

void UIButton::on_mouse_in(MouseInEvent* event) {
    UIView::on_mouse_in(event);
    update_state();
}

void UIButton::on_mouse_out(MouseOutEvent* event) {
    UIView::on_mouse_out(event);
    update_state();
}

void UIButton::on_mouse_down(MouseDownEvent* event) {
    UIView::on_mouse_down(event);
    event->mark_handled();
    transform.translate(0, 2);
    update_state();
}

void UIButton::on_mouse_up(MouseUpEvent* event) {
    UIView::on_mouse_up(event);
    event->mark_handled();
    transform.translate(0, -2);
    update_state();
}

void UIButton::on_mouse_click(MouseClickEvent* event) {
    if(callback) callback();
    event->mark_handled();
}

void UIButton::set_style(const UIButtonStyle* p_style) {
    Styled::set_style(p_style);
    update_state();
}

void UIButton::set_active(bool p_active) {
    UIView::set_active(p_active);
    update_state();
}

void UIButton::layout() {
    label->set_size(get_size());
}

UIButton::UIButton(const Vec2f &position, const Vec2f &size) : UIView(position, size) {
    append_child(label);
    set_style(UIButtonStyle::instance);
}

void UIButton::update_state() {
    if(!active) set_fill_style(style->get_inactive_color());
    else if(hovered) set_fill_style(style->get_hovered_color());
    else if(clicked) set_fill_style(style->get_clicked_color());
    else if(selected) set_fill_style(style->get_selected_color());
    else set_fill_style(style->get_idle_color());
}
