#pragma once

#include "../../../ui/ui_input.hpp"

class UINumberInput : public UIInput {

    std::function<void(void)> float_value_callback = {};
    float last_saved_value = 0;
    float min = 0;
    float max = 1;

    const char* number_format = "%.1f";

    void update_number_text() {
        int size = snprintf(nullptr, 0, number_format, last_saved_value) + 1;
        std::vector<char> buffer(size, 0);
        snprintf(&buffer[0], size, number_format, last_saved_value);
        set_contents(buffer);
    }

public:
    UINumberInput(const Vec2f& position = {}, const Vec2f& size = {}): UIInput(position, size) {
        update_number_text();
    }

    void on_key_down(KeyDownEvent *event) override {
        UIInput::on_key_down(event);
        if(event->is_handled()) return;

        if(event->code == KeyCode::up) adjust_value(event->shift ? 10 : 1);
        else if(event->code == KeyCode::down) adjust_value(event->shift ? -10 : -1);
    }

    void adjust_value(float delta) {
        set_float_value(get_float_value() + delta);
        update_number_text();
    }

    void handle_enter() override {
        if(contents.size() > 1) {
            char* end = nullptr;
            auto value = (float) strtod(&contents[0], &end);
            if(end == &(*contents.end()) - 1) {
                if(value < min) value = min;
                if(value > max) value = max;
                last_saved_value = value;
                if(float_value_callback) float_value_callback();
                if(enter_callback) enter_callback();
            }
        }
        update_number_text();
    }

    void set_float_value(float p_value) {
        last_saved_value = p_value;
        if(last_saved_value < min) last_saved_value = min;
        if(last_saved_value > max) last_saved_value = max;
        if(float_value_callback) float_value_callback();
        update_number_text();
    }

    float get_float_value() { return last_saved_value; }

    void set_min(float p_min) {
        min = p_min;
        if(min > last_saved_value) {
            last_saved_value = min;
            if(float_value_callback) float_value_callback();
            update_number_text();
        }
    }

    void set_max(float p_max) {
        max = p_max;
        if(max < last_saved_value) {
            last_saved_value = max;
            if(float_value_callback) float_value_callback();
            update_number_text();
        }
    }

    const char* get_number_format() const { return number_format; }
    void set_number_format(const char* p_number_format) { number_format = p_number_format; update_text(); }

    void set_float_value_callback(const std::function<void(void)> &p_callback) { float_value_callback = p_callback; }
};