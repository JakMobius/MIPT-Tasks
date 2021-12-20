#pragma once

struct UISelectOption;
class UISelectView;

#include "../ui_view.hpp"
#include "../ui_text.hpp"
#include "ui_select_modal.hpp"
#include "../styles/select_view_style.hpp"

struct UISelectOption {
    const char* title;
    std::function<void(void)> callback;
};

class UISelectView : public UIView, public Styled<UISelectViewStyle> {

    UIText* label = new UIText();
    std::vector<UISelectOption> options {};

public:
    explicit UISelectView(const Vec2f& position = {0, 0}, const Vec2f& size = {200, 50}): UIView(position, size) {
        set_style(UISelectViewStyle::instance);
        label->get_text_drawer()->set_h_alignment(HTextAlignmentLeft);
        label->set_position({10, 5});
        append_child(label);
    }

    void layout() override {
        UIView::layout();
        label->set_size(size - Vec2f { 20, 10 });
    }

    void draw(DrawingContext* ctx) override;
    void on_mouse_click(MouseClickEvent *event) override;

    void add_variant(const UISelectOption& option) {
        options.push_back(option);
    }

    void set_style(const UISelectViewStyle* p_style) override;
};
