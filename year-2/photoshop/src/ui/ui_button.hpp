#pragma once

#include "ui_view.hpp"
#include "ui_text.hpp"

typedef std::function<void()> button_callback;

struct UIButtonStyle {
    Vec4f hovered_color;
    Vec4f idle_color;
    Vec4f clicked_color;
    Vec4f selected_color;
    Vec4f disabled_color;
    Vec4f inactive_color;
};

extern const UIButtonStyle UI_DEFAULT_BUTTON_STYLE;

class UIButton : public UIView {
    UIText* label = new UIText();
    const UIButtonStyle* style = nullptr;
    button_callback callback {};

    bool selected = false;

    void update_state(bool active) {
        if(!active) set_background(style->inactive_color);
        else if(hovered) set_background(style->hovered_color);
        else if(clicked) set_background(style->clicked_color);
        else if(selected) set_background(style->selected_color);
        else set_background(style->idle_color);
    }

public:
    explicit UIButton(const Vec2f& position = {0, 0}, const Vec2f& size = {50, 50}): UIView(position, size) {
        append_child(label);
        set_style(&UI_DEFAULT_BUTTON_STYLE);
    }

    void set_title(const char* string) {
        label->set_text(string);
    }

    void on_mouse_in(MouseInEvent *event) override {
        UIView::on_mouse_in(event);
        set_needs_redraw();
    }

    void on_mouse_out(MouseOutEvent *event) override {
        UIView::on_mouse_out(event);
        set_needs_redraw();
    }

    void on_mouse_down(MouseDownEvent *event) override {
        UIView::on_mouse_down(event);
        event->mark_handled();
        transform.translate(0, 2);
        set_needs_redraw();
    }

    void on_mouse_up(MouseUpEvent *event) override {
        UIView::on_mouse_up(event);
        event->mark_handled();
        transform.translate(0, -2);
        set_needs_redraw();
    }

    void on_mouse_click(MouseClickEvent* event) override {
        if(callback) callback();
        event->mark_handled();
    }

    void draw(UIDrawingContext *ctx) override {
        update_state(ctx->get_context_active());
        UIView::draw(ctx);
    }

    button_callback get_callback() { return callback; }
    void set_callback(button_callback p_callback) { callback = p_callback; }

    void set_selected(bool p_selected) { selected = p_selected; set_needs_redraw(); }

    void set_style(const UIButtonStyle* p_style) {
        style = p_style;
        set_needs_redraw();
    }

    void layout() override {
        label->set_size(get_size());
    }
};