#pragma once

class UIScreen;

#include "ui_view.hpp"
#include "select/ui_select_modal.hpp"
#include "ui_modal_container.hpp"
#include "ui_modal_view.hpp"
#include "animations/ui_animation_controller.hpp"

class UIScreen : public UIView {
    UIView* view_container = new UIView();
    UIModalContainerView* modal_view_container = new UIModalContainerView();
    UIAnimationController* animation_controller = new UIAnimationController();

public:
    explicit UIScreen(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}) : UIView(position, size) {
        append_child(view_container);
        append_child(modal_view_container);

        modal_view_container->hide();
    }

    void prepare_to_draw(DrawingContext *ctx) override {
        UIView::prepare_to_draw(ctx);
        animation_controller->tick(1.f/60.f);
    }

    void layout() override {
        view_container->set_size(get_size());
        modal_view_container->set_size(get_size());
    }

    UIAnimationController* get_animation_controller() { return animation_controller; }

    void handle_child_blur() override { current_focused_child = nullptr; }

    UIView* get_view_container() { return view_container; }
    UIModalContainerView* get_modal_container() { return modal_view_container; }
    UIScreen* get_screen() override { return this; }

    void present_modal(UIModalView* modal_view);
};