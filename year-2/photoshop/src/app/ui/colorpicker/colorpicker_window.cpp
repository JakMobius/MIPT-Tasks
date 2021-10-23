//
// Created by Артем on 22.10.2021.
//

#include "colorpicker_window.hpp"

void ColorPickerWindow::setup_slider_gradient(UISlider* slider, GradientSliderStyle* style) {
    slider->set_own_style(style);
    auto gradient = style->get_gradient();
    gradient->set_end_anchor({.color = {0, 0, 0, 1}, .position = {slider->get_size()[0], 0 }});
    gradient->set_start_anchor  ({.color = {0, 0, 0, 1}, .position = { }});
}
