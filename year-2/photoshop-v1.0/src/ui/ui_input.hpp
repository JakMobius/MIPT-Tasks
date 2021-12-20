#pragma once

#include "ui_view.hpp"
#include "styled.hpp"
#include "../utils/dispatch_queue.hpp"
#include "ui_insets.hpp"
#include <vector>

class UIInputStyle {
public:
    static UIInputStyle* instance;

    virtual Vec4f                get_text_color()       const { return Vec4f {0, 0, 0, 1}; };
    virtual const UIFillStyle*   get_background_color() const { static UIFillStyleColor color({1, 1, 1, 1}); return &color; }
    virtual const UIFillStyle*   get_focused_background_color() const { static UIFillStyleColor color({0.8, 0.8, 1, 1}); return &color; }
    virtual const UIFillStyle*   get_cursor_fill_style() const { static UIFillStyleColor color({0, 0, 0, 1}); return &color; }
    virtual const UIFillStyle*   get_selection_color() const { static UIFillStyleColor color({0.5, 0.5, 0.9, 1.0}); return &color; }
};

struct UIInputSelection {
    int position = 0;
    int anchor = -1;

    int get_lower() const;
    int get_upper() const;

    bool has_selection() { return anchor != -1; }
};

class UIInput : public UIView, public Styled<UIInputStyle> {

protected:
    TextDrawer text_drawer {};

    std::vector<char> contents {'\0'};
    DispatchQueueTaskHandle blink_task_handle = -1;
    UIInsets text_insets { 5 };

    std::function<void(void)> change_callback {};
    std::function<void(void)> enter_callback {};

    UIInputSelection cursor {};
    bool cursor_visible = false;

    void update_style();
    void update_text();
    void blink();
    void enable_cursor_blinking();
    void disable_cursor_blinking();

public:
    UIInput(const Vec2f& position = {}, const Vec2f& size = {}): UIView(position, size) {
        text_drawer.set_v_alignment(VTextAlignmentCenter);
        focusable = true;
    }
    ~UIInput() override;

    void set_style(const UIInputStyle *p_style) override;
    void layout() override;

    void draw(DrawingContext* ctx) override;

    void on_text_enter(TextEnterEvent* event) override;
    void on_key_down(KeyDownEvent *event) override;
    void on_key_up(KeyUpEvent *event) override;

    void on_mouse_down(MouseDownEvent* event) override;
    void on_mouse_move(MouseMoveEvent* event) override;

    void focus() override;
    void blur() override;

    void move_cursor_left(bool shift);
    void move_cursor_right(bool shift);

    const UIInsets& get_text_insets() const { return text_insets; }
    void set_text_insets(const UIInsets &p_text_insets) { text_insets = p_text_insets; set_needs_layout(); }

    void set_change_callback(const std::function<void(void)>& p_change_callback) { change_callback = p_change_callback; }
    void set_enter_callback(const std::function<void(void)>& p_enter_callback) { enter_callback = p_enter_callback; }

    TextDrawer* get_text_drawer() { return &text_drawer; }

    const std::vector<char>& get_contents() const { return contents; }
    void set_contents(const std::vector<char>& p_contents);
    void set_contents(const char* p_contents);

    void handle_backspace();
    virtual void handle_enter();

    void select_all();

    void copy_selection();

    void paste_from_clipboard();

    void jump_to_start(bool shift);

    void jump_to_end(bool shift);
};