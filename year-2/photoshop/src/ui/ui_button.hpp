#pragma once

#include "ui_view.hpp"
#include "ui_text.hpp"

typedef std::function<void()> button_callback;

class UIButton : public UIView {
    UIText label;
    button_callback callback {};

    bool hovered = false;
    bool selected = false;

    void update_state() {
        if(hovered) {
            set_background({0, 1, 0, 1});
        } else if(clicked) {
            set_background({0, 0.8, 0, 1});
        } else if(selected) {
            set_background({0.6, 0.6, 0.8, 1});
        } else {
            set_background({0.8, 0.8, 0.8, 1});
        }
    }

public:
    explicit UIButton(const Vec2f& position = {0, 0}, const Vec2f& size = {50, 50}): UIView(position, size), label({0, 0}, size) {
        children.push(&label);
        update_state();
    }

    ~UIButton() override = default;

    void set_title(const char* string) {
        label.set_text(string);
    }

    void on_mouse_in(MouseInEvent *event) override {
        UIView::on_mouse_in(event);
        hovered = true;
        update_state();
    }

    void on_mouse_out(MouseOutEvent *event) override {
        UIView::on_mouse_out(event);
        hovered = false;
        update_state();
    }

    void on_mouse_down(MouseDownEvent *event) override {
        UIView::on_mouse_down(event);
        transform.translate(0, 2);
        update_state();
    }

    void on_mouse_up(MouseUpEvent *event) override {
        UIView::on_mouse_up(event);
        transform.translate(0, -2);
        update_state();
    }

    void on_mouse_click(MouseClickEvent* event) override {
        if(callback) callback();
        event->mark_handled();
    }

    button_callback get_callback() { return callback; }
    void set_callback(button_callback callback) { this->callback = callback; }

    void set_selected(bool p_selected) { selected = p_selected; update_state(); }
};