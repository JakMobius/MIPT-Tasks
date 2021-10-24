#pragma once

class UIWindowHeaderView;
class UIWindow;

#include "../ui_stack.hpp"
#include "../ui_button.hpp"
#include "ui_window.hpp"
#include "../styles/window_style.hpp"

class UIWindowHeaderView : public UIStackView {
    UIStackView* buttons_container = new UIStackView(UIStackViewDirection::x);
    UIText* header = new UIText();
    UIView* spacer = new UIView();
    UIButton* close_button = new UIButton({}, {32, 32});
    UIButton* fullscreen_button = new UIButton({}, {32, 32});
    UIWindow* window = nullptr;
    Vec2f drag_start_position {};
    const UIWindowStyle* style;

    void setup_buttons_container();

public:
    explicit UIWindowHeaderView(UIWindow* window);

    void set_title(const char* string) { header->set_text(string); }

    void layout() override;

    void on_mouse_down(MouseDownEvent* event) override;
    void on_mouse_move(MouseMoveEvent* event) override;

    void update_style();

    const UIWindowStyle* get_style() { return style; }
};