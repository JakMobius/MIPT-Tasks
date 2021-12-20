#pragma once

#include "../../ui/ui_view.hpp"
#include "raw_host_widget.hpp"

class ProxyView : public UIView {
    RawHostWidget* widget = nullptr;
    Vec2f old_position {};
    bool position_valid = false;
public:
    ProxyView(RawHostWidget* widget): UIView(), widget(widget) {}

    void on_mouse_down(MouseDownEvent *event) override;
    void on_mouse_up(MouseUpEvent *event) override;
    void on_mouse_move(MouseMoveEvent *event) override;
    void on_mouse_out(MouseOutEvent *event) override;
    void on_mouse_scroll(MouseScrollEvent* event) override;

    void on_key_down(KeyDownEvent* event) override;
    void on_key_up(KeyUpEvent* event) override;

    void draw(DrawingContext* ctx) override;

    void on_text_enter(TextEnterEvent* event) override;
};