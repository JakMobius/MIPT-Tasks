#pragma once

#include "ui_view.hpp"
#include "ui_text.hpp"
#include "styles/button_style.hpp"
#include "styled.hpp"

typedef std::function<void()> button_callback;

class UIButton : public UIView, public Styled<UIButtonStyle> {
protected:
    UIText* label = new UIText();

    button_callback callback {};

    bool selected = false;
    bool enabled = true;

    void update_state();

public:
    explicit UIButton(const Vec2f& position = {0, 0}, const Vec2f& size = {50, 50});

    void set_title(const char* string) {
        label->get_text_drawer()->set_text(string);
    }

    void on_mouse_in(MouseInEvent *event) override;
    void on_mouse_out(MouseOutEvent *event) override;
    void on_mouse_down(MouseDownEvent *event) override;
    void on_mouse_up(MouseUpEvent *event) override;
    void on_mouse_click(MouseClickEvent* event) override;

    button_callback get_callback() { return callback; }
    void set_callback(const button_callback& p_callback) { callback = p_callback; }
    void set_selected(bool p_selected) { selected = p_selected; update_state(); }
    void set_style(const UIButtonStyle* p_style) override;

    void set_active(bool p_is_active) override {
        UIView::set_active(p_is_active);
        update_state();
    }

    void set_enabled(bool p_enabled);

    void layout() override;
};