#pragma once

class UIScreen;

#include "ui_view.hpp"
#include "ui_select_modal.hpp"
#include "ui_modal_container.hpp"

class UIScreen : public UIView {
    UIView view_container {};
    UIModalContainerView modal_view_container {};

public:
    explicit UIScreen(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}) : UIView(position, size) {
        append_child(&view_container);
        append_child(&modal_view_container);

        modal_view_container.hide();
    }

    void layout() override {
        view_container.set_size(get_size());
        modal_view_container.set_size(get_size());
    }

    UIView* get_view_container() { return &view_container; }
    UIModalContainerView* get_modal_container() { return &modal_view_container; }
    UIScreen* get_screen() override { return this; }

    void present_modal(UISelectModalView* modal_view, UIView* parent);
};