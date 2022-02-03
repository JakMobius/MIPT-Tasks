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
    slider->set_size(m_size - Vec2f {textbox_width + padding, 0});
    slider->set_position({0, (m_size[1] - slider->get_size()[1]) / 2});
    input->set_position(Vec2f {m_size[0] - textbox_width, 0});
    input->set_size({textbox_width, m_size[1]});
    UIView::layout();
}

UIInputSlider::UIInputSlider(const Vec2f &position, const Vec2f &size, UISlider* slider, UINumberInput* input):
        UIView(position, size),
        slider(slider),
        input(input) {

    append_child(input);
    append_child(slider);

    slider->set_callback([this](float value) {
        if(ignore_callbacks) return;
        ignore_callbacks = true;
        this->input->set_float_value(value * (max - min) + min);
        if(callback) callback(this->input->get_float_value());
        ignore_callbacks = false;
    });

    input->set_float_value_callback([this]() {
        if(ignore_callbacks) return;
        ignore_callbacks = true;
        this->slider->set_fraction((this->input->get_float_value() - min) / (max - min));
        if(callback) callback(this->input->get_float_value());
        ignore_callbacks = false;
    });
}
