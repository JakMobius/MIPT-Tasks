//
// Created by Артем on 07.11.2021.
//

#include "ui_input_slider.hpp"

void UIInputSlider::set_textbox_width(float width) {
    textbox_width = 50;
    set_needs_layout();
}

void UIInputSlider::set_padding(float p_padding) {
    padding = p_padding;
    set_needs_layout();
}

void UIInputSlider::layout() {
    slider->set_size(size - Vec2f {textbox_width + padding, 0});
    slider->set_position({0, (size[1] - slider->get_size()[1]) / 2});
    input->set_position(Vec2f {size[0] - textbox_width, 0});
    input->set_size({textbox_width, size[1]});
    UIView::layout();
}

void UIInputSlider::update_input() {
    float value = slider->get_fraction() * (max - min) + min;
    int size = snprintf(nullptr, 0, number_format, value) + 1;
    std::vector<char> buffer(size, 0);
    snprintf(&buffer[0], size, number_format, value);
    input->set_contents(buffer);
}

UIInputSlider::UIInputSlider(const Vec2f &position, const Vec2f &size, UISlider* slider, UIInput* input):
        UIView(position, size),
        slider(slider),
        input(input) {

    append_child(input);
    append_child(slider);

    slider->set_callback([this](float value) {
        if(ignore_callbacks) return;
        ignore_callbacks = true;
        update_input();
        if(callback) callback(value * (max - min) + min);
        ignore_callbacks = false;
    });

    input->set_enter_callback([this]() {
        if(ignore_callbacks) return;
        ignore_callbacks = true;
        auto &contents = this->input->get_contents();
        if(contents.size() > 1) {
            char* end = nullptr;
            auto value = (float) strtod(&contents[0], &end);
            if(end == &(*contents.end()) - 1) {
                if(value < min) value = min;
                if(value > max) value = max;
                this->slider->set_fraction((value - min) / (max - min));
            }
        }
        update_input();
        if(callback) callback(this->slider->get_fraction() * (max - min) + min);
        ignore_callbacks = false;
    });

    update_input();
}
