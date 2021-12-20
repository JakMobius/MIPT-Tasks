#pragma once

#include "../photoshop_window.hpp"
#include "hue_saturation_select_view.hpp"
#include "ui_color_slider.hpp"
#include "../../../ui/styles/fill_style/fill_style_gradient.hpp"

class GradientSliderStyle : public UISliderStyle {
    UIFillStyleGradient* gradient = new UIFillStyleGradient();
    UIFillStyleTexture* texture = new UIFillStyleTexture();
    UIFillStyleColor* button_color = new UIFillStyleTexture(Assets.slider_bar_texture);
public:
    ~GradientSliderStyle() override { delete gradient; delete texture; delete button_color; }

    UIFillStyleGradient* get_gradient() { return gradient; }

    const UIFillStyle* get_button_color() const override { return button_color; }
    const UIFillStyle* get_bar_color() const override { return texture; }
    float get_bar_height() const override { return 10; };
    Vec2f get_button_size() const override { return {12, 24}; }

    void set_bar_texture(Textured* p_texture) { texture->set_texture(p_texture); }
};

class UIColorInputStyle : public UIInputStyle {
public:
    static UIColorInputStyle* instance;

    Vec4f                get_text_color()       const override { return Vec4f {0.702,  0.69,  0.694,  1}; };
    const UIFillStyle*   get_background_color() const override { static UIFillStyleColor color({0, 0, 0, 0}); return &color; }
    const UIFillStyle*   get_focused_background_color() const override { static UIFillStyleColor color({0.1, 0.1, 0.1, 1}); return &color; }
    const UIFillStyle*   get_cursor_fill_style() const override { static UIFillStyleColor color({1, 1, 1, 1}); return &color; }
};

class ColorPickerWindow: public PhotoshopWindow {
    std::function<void(Vec4f)> callback;

    UIStackView* container = nullptr;
    UIView* hsv_container = nullptr;

    GradientSliderStyle* value_slider_style = nullptr;
    UIColorSlider* value_slider = nullptr;

    GradientSliderStyle* rgba_slider_styles[4] {};
    UIColorSlider* rgba_sliders[4] {};

    HueSaturationSelectView* hue_saturation_view = nullptr;

    Vec3f current_rgb {};
    Vec3f current_hsv {};
    float current_alpha = 0;

    bool callbacks_ignored = false;
    bool first_layout = false;

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

    void set_callback(const std::function<void(const Vec4f&)>& p_callback);
};