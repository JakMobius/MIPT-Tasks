#pragma once

#include "ui/ui_view.hpp"
#include "ui/ui_slider.hpp"
#include "ui/ui_button.hpp"

class InterfaceView : public UIView {
    UISlider slider           {{0, 0},  {300, 0}};
    UIButton animation_button {{0, 50}, {300, 50}};
public:
    InterfaceView(const Vec2d& position);

    UISlider* get_slider() { return &slider; }
    UIButton* get_animation_button() { return &animation_button; }
};