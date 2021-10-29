//
// Created by Артем on 22.10.2021.
//

#include "colorpicker_window.hpp"

GradientSliderStyle* ColorPickerWindow::create_slider_style(UIColorSlider* slider) {
    auto* style = new GradientSliderStyle();
    auto* gradient = style->get_gradient();

    gradient->set_end_anchor  ({.color = {0, 0, 0, 1}, .position = {slider->get_size()[0], 0 }});
    gradient->set_start_anchor({.color = {0, 0, 0, 1}, .position = { }});

    slider->set_own_style(style);
    slider->get_bar_gradient_view()->set_fill_style(gradient);
    return style;
}

void ColorPickerWindow::add_rgba_slider(int index) {
    auto stack = new UIStackView(UIStackViewDirection::x);
    stack->set_item_spacing(20);

    stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
    stack->set_lateral_alignment(UIStackViewLateralAlignment::center);

    auto* slider = new UIColorSlider({}, {300, 40});
    auto* text_label = new UIText({}, {40, 10});

    char* label_text = rgb_label_values + index * 4;
    strcpy(label_text, "0");
    text_label->get_text_drawer()->set_text(label_text);
    text_label->get_text_drawer()->set_font_color({1, 1, 1, 1});

    stack->append_child(slider);
    stack->append_child(text_label);

    slider->set_callback([label_text, text_label, index, this](float value) {
        snprintf(label_text, 4, "%d", (int)(value * 255));
        text_label->get_text_drawer()->set_text(label_text);
        this->on_rgb_slider_updated(index, value);
    });

    rgba_sliders[index] = slider;
    rgba_slider_styles[index] = create_slider_style(slider);
    container->append_child(stack);
}

void ColorPickerWindow::setup_hue_saturation_view() {
    hue_saturation_view = new HueSaturationSelectView({0, 0}, {300, 300});
    hue_saturation_view->set_callback([this](float hue, float saturation) {
        on_hue_saturation_update(hue, saturation);
    });
    hsv_container->append_child(hue_saturation_view);
}

void ColorPickerWindow::setup_value_slider() {
    value_slider = new UIColorSlider({360, 0}, {300, 40});
    value_slider_style = create_slider_style(value_slider);
    value_slider->set_transform(Matrix3f::rotation_matrix(-M_PI / 2));
    value_slider->set_callback([this](float value) { this->on_value_slider_updated(value); });
    hsv_container->append_child(value_slider);
}

ColorPickerWindow::ColorPickerWindow(PhotoshopView* app, const Vec2f &position) : PhotoshopWindow(app, position, {400, 500}, "Color picker") {

    container = new UIStackView(UIStackViewDirection::y);
    container->set_insets({20});
    container->set_item_spacing(10);
    hsv_container = new UIView({}, {360, 300});
    container->append_child(hsv_container);

    setup_hue_saturation_view();
    setup_value_slider();
    for(int i = 0; i < 4; i++) add_rgba_slider(i);

    rgba_slider_styles[3]->set_bar_texture(Assets.canvas_background_texture);

    get_content_view()->append_child(container);

    set_color({1, 0, 1, 1});
}

void ColorPickerWindow::set_color(const Vec4f &color) {
    current_rgb.content = color.content;
    current_hsv = hsv_from_rgb(current_rgb);
    current_alpha = color[3];

    update_value_slider();
    update_rgba_sliders();
    update_hue_saturation_view();

    call_callback();
}

void ColorPickerWindow::update_rgba_sliders() {
    update_rgba_slider_gradients();
    for(int i = 0; i < 3; i++) {
        rgba_sliders[i]->set_fraction(current_rgb[i]);
    }
    rgba_sliders[3]->set_fraction(current_alpha);
}

void ColorPickerWindow::update_rgba_slider_gradients() {
    for(int i = 0; i < 4; i++) {
        auto gradient = rgba_slider_styles[i]->get_gradient();
        auto* start_color = &gradient->get_start_anchor()->color;
        auto* end_color = &gradient->get_end_anchor()->color;

        end_color->content = current_rgb.content;
        end_color->set_w(1);
        end_color->set(i, 1);

        start_color->content = current_rgb.content;
        start_color->set_w(1);
        start_color->set(i, 0);
    }
}

void ColorPickerWindow::update_value_slider() {
    update_value_slider_gradient();
    value_slider->set_fraction(1 - current_hsv[2]);
}

void ColorPickerWindow::update_value_slider_gradient() {
    Vec3f max_valued_color = current_hsv;
    max_valued_color.set_z(1);
    max_valued_color = rgb_from_hsv(max_valued_color);

    auto* slider_color = &value_slider_style->get_gradient()->get_start_anchor()->color;
    slider_color->content = max_valued_color.content;
    slider_color->set_w(1);
}

void ColorPickerWindow::update_hue_saturation_view() {
    hue_saturation_view->set_value(current_hsv[0], current_hsv[1]);
}

void ColorPickerWindow::on_value_slider_updated(float value) {
    if(callbacks_ignored) return;
    callbacks_ignored = true;

    current_hsv.set_z(1 - value);
    current_rgb = rgb_from_hsv(current_hsv);
    update_rgba_sliders();

    call_callback();
    callbacks_ignored = false;
}

void ColorPickerWindow::on_rgb_slider_updated(int index, float value) {
    if(callbacks_ignored) return;
    callbacks_ignored = true;

    if(index < 3) {
        current_rgb.set(index, value);
        current_hsv = hsv_from_rgb(current_rgb);
        update_value_slider();
        update_rgba_slider_gradients();
        update_hue_saturation_view();
    } else current_alpha = value;

    call_callback();
    callbacks_ignored = false;
}

void ColorPickerWindow::on_hue_saturation_update(float hue, float saturation) {
    if(callbacks_ignored) return;
    callbacks_ignored = true;

    current_hsv.set_x(hue);
    current_hsv.set_y(saturation);
    current_rgb = rgb_from_hsv(current_hsv);
    update_rgba_sliders();
    update_value_slider_gradient();

    call_callback();
    callbacks_ignored = false;
}

void ColorPickerWindow::layout() {
    container->layout_if_needed();
    get_content_view()->set_size(container->get_size());
    UIWindow::layout();
}

void ColorPickerWindow::call_callback() {
    if(callback) {
        Vec4f color(current_rgb.content);
        color.set_w(current_alpha);
        callback(color);
    }
}

