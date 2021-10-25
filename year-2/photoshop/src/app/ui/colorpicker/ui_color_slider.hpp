#pragma once

#include "../../../ui/ui_slider.hpp"
#include "../../../graphics/shapes/rounded_rect_shape.hpp"

class UIColorSlider : public UISlider {
    UIView* bar_gradient_view;
public:
    UIColorSlider(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UISlider(position, size) {
        bar_gradient_view = new UIView();
        bar->append_child(bar_gradient_view);
    };

    void layout() override {
        UISlider::layout();
        bar_gradient_view->set_size(bar->get_size());
    }

    UIView* get_bar_gradient_view() { return bar_gradient_view; }
};