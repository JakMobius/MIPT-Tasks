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
    if(!enabled) return;
    event->mark_handled();
    transform.translate(0, 2);
    update_state();
}

void UIButton::on_mouse_up(MouseUpEvent* event) {
    bool was_enabled = enabled;
    UIView::on_mouse_up(event);
    if(!was_enabled) return;
    event->mark_handled();
    transform.translate(0, -2);
    update_state();
}

void UIButton::on_mouse_click(MouseClickEvent* event) {
    UIView::on_mouse_click(event);
    if(!enabled) return;
    if(callback) callback();
    event->mark_handled();
}

void UIButton::set_style(const UIButtonStyle* p_style) {
    Styled::set_style(p_style);
    update_state();
}

void UIButton::layout() {
    if(!style) set_style(UIButtonStyle::instance);
    label->set_size(get_size());
}

UIButton::UIButton(const Vec2f &position, const Vec2f &size) : UIView(position, size) {
    append_child(label);
    label->get_text_drawer()->set_font_color({0, 0, 0, 1});
    label->get_text_drawer()->set_font_size(16);
    label->get_text_drawer()->set_h_alignment(HTextAlignmentCenter);
    label->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
}

void UIButton::update_state() {
    if(!style) set_fill_style(nullptr);
    else if(!enabled) set_fill_style(style->get_disabled_color(), 0.3);
    else if(!active) set_fill_style(style->get_inactive_color(), 0.3);
    else if(hovered) set_fill_style(style->get_hovered_color(), 0.3);
    else if(clicked) set_fill_style(style->get_clicked_color(), 0.3);
    else if(selected) set_fill_style(style->get_selected_color(), 0.3);
    else set_fill_style(style->get_idle_color(), 0.3);
}

void UIButton::set_enabled(bool p_enabled) {
    if(!p_enabled && enabled && clicked) {
        transform.translate(0, -2);
    }
    enabled = p_enabled;
    update_state();
}
