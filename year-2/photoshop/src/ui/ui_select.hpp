#pragma once

struct UISelectOption;
class UISelectView;

#include "ui_view.hpp"
#include "ui_text.hpp"
#include "ui_select_modal.hpp"

struct UISelectOption {
    const char* title;
};

class UISelectView : public UIView {

    UIText label {};
    dynamic_array<UISelectOption> options {};

public:
    explicit UISelectView(const Vec2f& position = {0, 0}, const Vec2f& size = {200, 50}): UIView(position, size) {
        set_background({0.7, 0.7, 0.7, 1.0});
        label.set_h_text_alignment(HTextAlignmentLeft);
        label.set_position({10, 5});
        append_child(&label);
    }

    void layout() override {
        UIView::layout();
        label.set_size(size - Vec2f { 20, 10 });
    }

    void draw(DrawingContext* ctx) override;
    void on_mouse_click(MouseClickEvent *event) override;

    void add_variant(const UISelectOption& option) {
        options.push(option);
    }
};
