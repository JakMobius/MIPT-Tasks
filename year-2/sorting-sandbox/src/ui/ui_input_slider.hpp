#pragma once

#include "ui_input.hpp"
#include "ui_slider.hpp"
#include "ui_stack.hpp"
#include "../app/ui/colorpicker/ui_number_input.hpp"

class UIInputSlider : public UIView {
    std::function<void(float)> callback {};
    UINumberInput* input;
    UISlider* slider;

    float textbox_width = 50;
    float padding = 10;
    float min = 0;
    float max = 1;

    bool ignore_callbacks = false;

public:

    explicit UIInputSlider(const Vec2f &position = {}, const Vec2f &size = {}, UISlider* slider = new UISlider(), UINumberInput* input = new UINumberInput());

    void set_textbox_width(float width);
    void set_padding(float p_padding);

    void layout() override;

    UINumberInput* get_input() const { return input; }

    UISlider* get_slider() const { return slider; }

    void set_callback(const std::function<void(float)> &p_callback) { callback = p_callback; }

    void set_min(float p_min) { min = p_min; input->set_min(min); }
    void set_max(float p_max) { max = p_max; input->set_max(max); }
};