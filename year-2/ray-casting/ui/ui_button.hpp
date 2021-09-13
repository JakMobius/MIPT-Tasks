#pragma once

#include "ui_view.hpp"
#include "ui_text.hpp"

class UIButton : public UIView {
    UIText label;

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
        printf("hovered %s\n", label.get_text());
    }

    void on_mouse_out(MouseOutEvent *event) override {
        UIView::on_mouse_out(event);

        background = {1, 1, 1, 1};
    }

    void on_mouse_down(MouseDownEvent *event) override {
        transform.translate(0, 2);
    }

    void on_mouse_up(MouseUpEvent *event) override {
        transform.translate(0, -2);
    }
};