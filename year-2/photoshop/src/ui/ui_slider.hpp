#pragma once

#include "ui_view.hpp"
#include "styles/slider_style.hpp"
#include "styled.hpp"

typedef std::function<void(float)> ui_slider_callback;

class UISlider : public UIView, public Styled<UISliderStyle> {
protected:
    UIView* bar = new UIView();
    UIView* button = new UIView();
    float fraction = 0.5;
    ui_slider_callback callback {};
    float mouse_click_point = 0;

    void layout() override;

    void set_button_position(float x);

public:
    UISlider(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void set_size(const Vec2f &new_size) override;

    void on_mouse_down(MouseDownEvent *event) override;
    void on_mouse_move(MouseMoveEvent *event) override;

    float get_fraction() const { return fraction; }
    ui_slider_callback get_callback() { return callback; }
    void set_callback(ui_slider_callback p_callback) { callback = p_callback; }

    void set_fraction(float fraction);
    void set_style(const UISliderStyle* p_style) override;

    UIView* get_bar() { return bar; }
};