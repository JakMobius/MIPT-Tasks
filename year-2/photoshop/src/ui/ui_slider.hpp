#pragma once

#include "ui_view.hpp"

typedef std::function<void(float)> ui_slider_callback;

class UISlider : public UIView {
    UIView* bar = new UIView();
    UIView* button = new UIView();
    float fraction = 0.5;
    ui_slider_callback callback {};
    float mouse_click_point = 0;

    void layout() override {
        UIView::layout();
        bar->set_position({0, BUTTON_HEIGHT / 2 - BAR_HEIGHT / 2});
        bar->set_size({size[0], BAR_HEIGHT});
        float position_x = (size[0] - BUTTON_WIDTH) * fraction;
        button->set_position({position_x, 0});
    }

    void set_button_position(float x) {
        float max_right = size[0] - BUTTON_WIDTH;

        Vec2f button_position = button->get_position();
        button_position.set_x(x);
        if(button_position[0] < 0) button_position.set_x(0);
        if(button_position[0] > max_right) button_position.set_x(max_right);
        button->set_position(button_position);

        fraction = button_position[0] / (size[0] - BUTTON_WIDTH);
        if(callback) callback(fraction);
    }

public:
    UISlider(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void set_size(const Vec2f &new_size) override {
        Vec2f real_size = {new_size[0], BUTTON_HEIGHT};
        UIView::set_size(real_size);
    }

    void on_mouse_down(MouseDownEvent *event) override {
        UIView::on_mouse_down(event);
        if(current_clicked_child != button) {
            set_button_position(event->x - BUTTON_WIDTH / 2);
            mouse_click_point = BUTTON_WIDTH / 2;
        } else {
            mouse_click_point = event->x - button->get_position()[0];
        }
    }

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
};