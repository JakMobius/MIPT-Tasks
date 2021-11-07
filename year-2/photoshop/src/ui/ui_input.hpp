#pragma once

#include "ui_view.hpp"
#include "styled.hpp"
#include "../utils/dispatch_queue.hpp"
#include <vector>

class UIInputStyle {
public:
    static UIInputStyle* instance;

    virtual const Vec4f          get_text_color()       const { return Vec4f {0, 0, 0, 1}; };
    virtual const UIFillStyle*   get_background_color() const { static UIFillStyleColor color({1, 1, 1, 1}); return &color; }
    virtual const UIFillStyle*   get_focused_background_color() const { static UIFillStyleColor color({0.8, 0.8, 1, 1}); return &color; }
};

class UIInput : public UIView, public Styled<UIInputStyle> {
    TextDrawer text_drawer {};
    std::vector<char> contents {'\0'};
    DispatchQueueTaskHandle blink_task_handle;
    UIFillStyleColor cursor_fill_style {{ 0, 0, 0, 1 }};

    int cursor_position = 0;
    bool cursor_visible;

    void update_style();
    void update_text();
    void blink();
    void enable_blink();
    void disable_blink();

public:
    UIInput(const Vec2f& position, const Vec2f& size): UIView(position, size) {}

    void set_style(const UIInputStyle *p_style) override;
    void layout() override;

    void draw(DrawingContext* ctx) override;

    void on_text_enter(TextEnterEvent* event) override;
    void on_key_down(KeyDownEvent *event) override;
    void on_key_up(KeyUpEvent *event) override;

    void focus() override;
    void blur() override;
};