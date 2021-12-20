#pragma once

#include "../../ui/ui_button.hpp"

class ActionButton : public UIButton {
public:
    explicit ActionButton(const char* action = nullptr, const std::function<void(void)>& callback = nullptr): UIButton() {
        if(action) set_title(action);
        label->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
        label->get_text_drawer()->set_h_alignment(HTextAlignmentCenter);
        label->get_text_drawer()->set_font_size(18);
        set_callback(callback);
    }

    void layout() override {
        set_size({ label->get_text_drawer()->get_text_bounds()[0] + 40, 40 });
        UIButton::layout();
    }
};