#pragma once

#include "../../ui/window/ui_window.hpp"

class ToolSelectWindow : public UIWindow {
    UIStackView* stack = nullptr;

    void create_tool_buttons() {
        for(int i = 0; i < 10; i++) {
            auto* button = new UIButton({}, {50, 50});
            stack->append_child(button);
        }
    }

public:
    explicit ToolSelectWindow(const Vec2f& position): UIWindow(position, {}, "") {
        stack = new UIStackView(UIStackViewDirection::y);
        stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
        stack->set_item_spacing(7);
        stack->set_insets({7});
        stack->set_fitting({});

        stack->set_background({1, 1, 1, 1});

        create_tool_buttons();

        get_content_view()->append_child(stack);
    };

    void layout() override {
        stack->layout_if_needed();
        get_content_view()->set_size(stack->get_size());
        UIWindow::layout();
    }
};