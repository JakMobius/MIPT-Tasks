#pragma once

#include "../photoshop_window.hpp"
#include "hue_saturation_select_view.hpp"
#include "../../../ui/ui_slider.hpp"

class GradientSliderStyle : public UISliderStyle {
    UIFillStyleGradient* gradient = new UIFillStyleGradient();
public:
    ~GradientSliderStyle() override { delete gradient; }

    UIFillStyleGradient* get_gradient() { return gradient; }

    const UIFillStyle * get_bar_color() const override { return gradient; }
};

class ColorPickerWindow: public PhotoshopWindow {
    std::function<void(Vec4f)> callback;
    GradientSliderStyle* red_slider_style = new GradientSliderStyle();
    GradientSliderStyle* green_slider_style = new GradientSliderStyle();
    GradientSliderStyle* blue_slider_style = new GradientSliderStyle();
    GradientSliderStyle* value_slider_style = new GradientSliderStyle();

    UISlider* value_slider = nullptr;
    UISlider* red_slider = nullptr;
    UISlider* green_slider = nullptr;
    UISlider* blue_slider = nullptr;

    HueSaturationSelectView* hue_saturation_view = nullptr;

    Vec3f current_rgb {};
    Vec3f current_hsv {};
    float current_alpha = 0;

    bool callbacks_ignored = false;

    void setup_slider_gradient(UISlider* slider, GradientSliderStyle* style);

public:
    ColorPickerWindow(PhotoshopView* app, const Vec2f& position): PhotoshopWindow(app, position, {400, 500}, "Color picker") {
        hue_saturation_view = new HueSaturationSelectView({10, 10}, {300, 300});
        value_slider = new UISlider({360, 10}, {300, 40});
        red_slider   = new UISlider({10, 340}, {300, 40});
        green_slider = new UISlider({10, 390}, {300, 40});
        blue_slider  = new UISlider({10, 440}, {300, 40});

        hue_saturation_view->set_callback([this](float hue, float saturation) {
            on_hue_saturation_update(hue, saturation);
        });

        value_slider->set_transform(Matrix3f::rotation_matrix(-M_PI / 2));

        setup_slider_gradient(value_slider, value_slider_style);
        setup_slider_gradient(red_slider, red_slider_style);
        setup_slider_gradient(green_slider, green_slider_style);
        setup_slider_gradient(blue_slider, blue_slider_style);

        red_slider  ->set_callback([this](float value) { this->on_rgb_slider_updated(0, value); });
        green_slider->set_callback([this](float value) { this->on_rgb_slider_updated(1, value); });
        blue_slider ->set_callback([this](float value) { this->on_rgb_slider_updated(2, value); });

        value_slider->set_callback([this](float value) { this->on_value_slider_updated(value); });

        auto content_view = get_content_view();

        content_view->append_child(hue_saturation_view);
        content_view->append_child(value_slider);
        content_view->append_child(red_slider);
        content_view->append_child(green_slider);
        content_view->append_child(blue_slider);

        set_color({1, 0, 1, 1});
    };

    void set_color(const Vec4f& color) {
        current_rgb.content = color.content;
        current_hsv = hsv_from_rgb(current_rgb[0], current_rgb[1], current_rgb[2]);
        current_alpha = color[3];

        update_value_slider();
        update_rgb_sliders();
        update_hue_saturation_view();
    }

    void update_rgb_sliders() {
        update_rgb_slider_gradients();
        red_slider  ->set_fraction(current_rgb[0]);
        green_slider->set_fraction(current_rgb[1]);
        blue_slider ->set_fraction(current_rgb[2]);
    }

    void update_rgb_slider_gradients() {
        red_slider_style  ->get_gradient()->get_end_anchor()->color = { 1, current_rgb[1], current_rgb[2], 1 };
        green_slider_style->get_gradient()->get_end_anchor()->color = { current_rgb[0], 1, current_rgb[2], 1 };
        blue_slider_style ->get_gradient()->get_end_anchor()->color = { current_rgb[0], current_rgb[1], 1, 1 };

        red_slider_style  ->get_gradient()->get_start_anchor()->color = { 0, current_rgb[1], current_rgb[2], 1 };
        green_slider_style->get_gradient()->get_start_anchor()->color = { current_rgb[0], 0, current_rgb[2], 1 };
        blue_slider_style ->get_gradient()->get_start_anchor()->color = { current_rgb[0], current_rgb[1], 0, 1 };
    }

    void update_value_slider() {
        update_value_slider_gradient();
        value_slider->set_fraction(1 - current_hsv[2]);
    }

    void update_value_slider_gradient() {
        Vec4f color {rgb_from_hsv(current_hsv[0], current_hsv[1], 1).content };
        color.set_w(1.0);
        value_slider_style->get_gradient()->get_start_anchor()->color = color;
    }

    void update_hue_saturation_view() {
        hue_saturation_view->set_value(current_hsv[0], current_hsv[1]);
    }

    void on_value_slider_updated(float value) {
        if(callbacks_ignored) return;
        callbacks_ignored = true;

        current_hsv.set_z(1 - value);
        current_rgb = rgb_from_hsv(current_hsv[0], current_hsv[1], current_hsv[2]);
        update_rgb_sliders();

        callbacks_ignored = false;
    }

    void on_rgb_slider_updated(int index, float value) {
        if(callbacks_ignored) return;
        callbacks_ignored = true;

        current_rgb.set(index, value);
        current_hsv = hsv_from_rgb(current_rgb[0], current_rgb[1], current_rgb[2]);
        update_value_slider();
        update_rgb_slider_gradients();
        update_hue_saturation_view();

        callbacks_ignored = false;
    }

    void on_hue_saturation_update(float hue, float saturation) {
        if(callbacks_ignored) return;
        callbacks_ignored = true;

        current_hsv.set_x(hue);
        current_hsv.set_y(saturation);
        current_rgb = rgb_from_hsv(current_hsv[0], current_hsv[1], current_hsv[2]);
        update_rgb_sliders();

        callbacks_ignored = false;
    }

    void set_callback(const std::function<void(const Vec4f&)>& p_callback) {
        callback = p_callback;
    }
};