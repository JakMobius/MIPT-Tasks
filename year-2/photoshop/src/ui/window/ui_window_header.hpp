#pragma once

class UIWindowHeaderView;
class UIWindow;

#include "../ui_stack.hpp"
#include "../ui_button.hpp"
#include "ui_window.hpp"

class UIWindowHeaderView : public UIStackView {
    UIStackView* buttons_container = new UIStackView(UIStackViewDirection::x);
    UIText* header = new UIText();
    UIView* spacer = new UIView();
    UIButton* close_button = new UIButton({}, {30, 30});
    UIButton* fullscreen_button = new UIButton({}, {30, 30});
    UIWindow* window = nullptr;
    Vec2f drag_start_position {};
    const UIWindowStyle* style;

    void setup_buttons_container();

public:
    explicit UIWindowHeaderView(UIWindow* window);

    void set_title(const char* string) {
        header->set_text(string);
    }

    void layout() override {
        buttons_container->layout_if_needed();
        spacer->set_size(buttons_container->get_size());
        UIStackView::layout();
    }

    void on_mouse_down(MouseDownEvent* event) override {
        UIStackView::on_mouse_down(event);
        drag_start_position = { event->x, event->y };
    }

    void on_mouse_move(MouseMoveEvent* event) override;

    void update_style();

    const UIWindowStyle* get_style() { return style; }
};