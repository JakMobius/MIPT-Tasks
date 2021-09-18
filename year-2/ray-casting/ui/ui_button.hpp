#pragma once

#include "ui_view.hpp"
#include "ui_text.hpp"

typedef std::function<void()> button_callback;

class UIButton : public UIView {
    UIText label;
    button_callback callback {};

public:
    UIButton(Vec2d position, Vec2d size): UIView(position, size), label({0, 0}, size) {
        children.push(&label);
    }

    ~UIButton() override = default;

    void set_title(const char* string) {
        label.set_text(string);
    }

    void on_mouse_in(MouseInEvent *event) override {
        UIView::on_mouse_in(event);

        background = {1, 0, 0, 1};
    }

    void on_mouse_out(MouseOutEvent *event) override {
        UIView::on_mouse_out(event);

        background = {1, 1, 1, 1};
    }

    void on_mouse_down(MouseDownEvent *event) override {
        UIView::on_mouse_down(event);
        transform.translate(0, 2);

    }

    void on_mouse_up(MouseUpEvent *event) override {
        UIView::on_mouse_up(event);
        transform.translate(0, -2);

        if(event->x >= 0 && event->y >= 0 && event->x < size.x && event->y < size.y) {
            callback();
        }
    }

    button_callback get_callback() { return callback; }
    void set_callback(button_callback callback) { this->callback = callback; }
};