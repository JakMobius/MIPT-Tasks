//
// Created by Артем on 12.11.2021.
//

#include "ui_number_input.hpp"

void UINumberInput::update_number_text() {
    int size = snprintf(nullptr, 0, number_format, last_saved_value) + 1;
    std::vector<char> buffer(size, 0);
    snprintf(&buffer[0], size, number_format, last_saved_value);
    set_contents(buffer);
}

void UINumberInput::on_key_down(KeyDownEvent* event) {
    UIInput::on_key_down(event);
    if(event->is_handled()) return;

    if(event->code == KeyCode::up) adjust_value(event->shift ? 10 : 1);
    else if(event->code == KeyCode::down) adjust_value(event->shift ? -10 : -1);
}

void UINumberInput::adjust_value(float delta) {
    set_float_value(get_float_value() + delta);
    update_number_text();
}

void UINumberInput::handle_enter() {
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

void UINumberInput::set_float_value(float p_value) {
    last_saved_value = p_value;
    if(last_saved_value < min) last_saved_value = min;
    if(last_saved_value > max) last_saved_value = max;
    if(float_value_callback) float_value_callback();
    update_number_text();
}

void UINumberInput::set_min(float p_min) {
    min = p_min;
    if(min > last_saved_value) {
        last_saved_value = min;
        if(float_value_callback) float_value_callback();
        update_number_text();
    }
}

void UINumberInput::set_max(float p_max) {
    max = p_max;
    if(max < last_saved_value) {
        last_saved_value = max;
        if(float_value_callback) float_value_callback();
        update_number_text();
    }
}
