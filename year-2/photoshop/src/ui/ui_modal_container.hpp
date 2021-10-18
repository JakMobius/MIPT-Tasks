#pragma once

#include "ui_view.hpp"

class UIModalContainerView : public UIView {
public:
    explicit UIModalContainerView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIView(position, size) {};

    void on_mouse_click(MouseClickEvent *event) override {
        if(!event->is_handled()) {
            hide();
        }
    }

    void present() { set_hidden(false); }
    void hide() {
        for(int i = children.size - 1; i >= 0; i--) {
            remove_child(i);
        }
        set_hidden(true);
    }
};