#pragma once

#include "../photoshop_window.hpp"
#include "hue_saturation_select_view.hpp"
#include "ui_color_slider.hpp"

class GradientSliderStyle : public UISliderStyle {
    UIFillStyleGradient* gradient = new UIFillStyleGradient();
    UIFillStyleTexture* texture = new UIFillStyleTexture();
public:
    ~GradientSliderStyle() override { delete gradient; delete texture; }

    UIFillStyleGradient* get_gradient() { return gradient; }

    const UIFillStyle * get_bar_color() const override { return texture; }
    float get_bar_height() const override { return 16; };

    void set_bar_texture(Drawable* p_texture) { texture->set_texture(p_texture); }
};

class ColorPickerWindow: public PhotoshopWindow {
    std::function<void(Vec4f)> callback;

    UIStackView* container = nullptr;
    UIView* hsv_container = nullptr;

    GradientSliderStyle* value_slider_style = nullptr;
    UIColorSlider* value_slider = nullptr;

    GradientSliderStyle* rgba_slider_styles[4] {};
    UIColorSlider* rgba_sliders[4] {};
    char rgb_label_values[16]{};

    HueSaturationSelectView* hue_saturation_view = nullptr;

    Vec3f current_rgb {};
    Vec3f current_hsv {};
    float current_alpha = 0;

    bool callbacks_ignored = false;

    static GradientSliderStyle* create_slider_style(UIColorSlider* slider);
    void add_rgba_slider(int index);
    void setup_hue_saturation_view();
    void setup_value_slider();

    void update_rgba_sliders();
    void update_rgba_slider_gradients();
    void update_value_slider();
    void update_value_slider_gradient();
    void update_hue_saturation_view();

    void call_callback();

public:
    ColorPickerWindow(PhotoshopView* app, const Vec2f& position);;

    void set_color(const Vec4f& color);
    void on_value_slider_updated(float value);
    void on_rgb_slider_updated(int index, float value);
    void on_hue_saturation_update(float hue, float saturation);
    void layout() override;

    void set_callback(const std::function<void(const Vec4f&)>& p_callback) {
        callback = p_callback;
    }
};