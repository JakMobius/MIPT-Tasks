#pragma once

#include "../../../ui/ui_input.hpp"

class UINumberInput : public UIInput {

    std::function<void(void)> float_value_callback = {};
    float last_saved_value = 0;
    float min = 0;
    float max = 1;

    const char* number_format = "%.1f";

    void update_number_text();

public:
    UINumberInput(const Vec2f& position = {}, const Vec2f& size = {}): UIInput(position, size) {
        update_number_text();
    }

    void on_key_down(KeyDownEvent *event) override;
    void adjust_value(float delta);

    void handle_enter() override;

    void set_float_value(float p_value);
    float get_float_value() { return last_saved_value; }

    void set_min(float p_min);
    void set_max(float p_max);

    const char* get_number_format() const { return number_format; }
    void set_number_format(const char* p_number_format) { number_format = p_number_format; update_text(); }
    void set_float_value_callback(const std::function<void(void)> &p_callback) { float_value_callback = p_callback; }
};