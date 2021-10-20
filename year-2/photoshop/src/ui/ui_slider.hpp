#pragma once

#include "ui_view.hpp"
#include "styles/slider_style.hpp"

typedef std::function<void(float)> ui_slider_callback;

class UISlider : public UIView {
    UIView* bar = new UIView();
    UIView* button = new UIView();
    float fraction = 0.5;
    ui_slider_callback callback {};
    float mouse_click_point = 0;
    const UISliderStyle* style = nullptr;

    void layout() override;

    void set_button_position(float x);

public:
    UISlider(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void set_size(const Vec2f &new_size) override {
        Vec2f real_size = {new_size[0], BUTTON_HEIGHT};
        UIView::set_size(real_size);
    }

    void on_mouse_down(MouseDownEvent *event) override;

    void on_mouse_up(MouseUpEvent *event) override {
        UIView::on_mouse_up(event);
    }

    void on_mouse_move(MouseMoveEvent *event) override;

    float get_fraction() const { return fraction; }
    ui_slider_callback get_callback() { return callback; }
    void set_callback(ui_slider_callback p_callback) { callback = p_callback; }

    constexpr const static float BUTTON_WIDTH = 40;
    constexpr const static float BUTTON_HEIGHT = 40;
    constexpr const static float BAR_HEIGHT = 10;

    void set_fraction(float fraction);
    const UISliderStyle* get_style() const { return style; }
    void set_style(const UISliderStyle* p_style);
};