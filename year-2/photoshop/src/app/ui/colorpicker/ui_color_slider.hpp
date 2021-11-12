#pragma once

#include "../../../ui/ui_slider.hpp"
#include "../../../graphics/shapes/rounded_rect_shape.hpp"
#include "../../../ui/ui_input_slider.hpp"

class UIColorSlider: public UISlider {
    UIView* bar_gradient_view;
public:
    UIColorSlider(const Vec2f& position = {}, const Vec2f& size = {}): UISlider(position, size) {
        bar_gradient_view = new UIView();
        get_bar()->append_child(bar_gradient_view);
    }

    void layout() override {
        UISlider::layout();
        bar_gradient_view->set_size(get_bar()->get_size());
    }

    UIView* get_bar_gradient_view() { return bar_gradient_view; }
};