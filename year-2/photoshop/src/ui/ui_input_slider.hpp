#pragma once

#include "ui_input.hpp"
#include "ui_slider.hpp"
#include "ui_stack.hpp"

class UIInputSlider : public UIView {
    std::function<void(float)> callback {};
    UIInput* input;
    UISlider* slider;
    const char* number_format = "%.1f";

    float textbox_width = 50;
    float padding = 10;
    float min = 0;
    float max = 1;

    bool ignore_callbacks = false;

    void update_input();

public:

    explicit UIInputSlider(const Vec2f &position = {}, const Vec2f &size = {}, UISlider* slider = new UISlider(), UIInput* input = new UIInput());

    void set_textbox_width(float width);
    void set_padding(float p_padding);

    void layout() override;

    UIInput* get_input() const { return input; }

    UISlider* get_slider() const { return slider; }

    void set_callback(const std::function<void(float)> &p_callback) { callback = p_callback; }

    void set_min(float p_min) { min = p_min; }
    void set_max(float p_max) { max = p_max; }

    const char* get_number_format() const { return number_format; }
    void set_number_format(const char* p_number_format) { number_format = p_number_format; }
};