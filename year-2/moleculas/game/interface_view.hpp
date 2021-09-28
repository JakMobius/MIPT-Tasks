#pragma once

#include "../visual-engine/ui/ui_view.hpp"
#include "../visual-engine/ui/ui_slider.hpp"
#include "../visual-engine/ui/ui_button.hpp"

class InterfaceView : public UIView {
    UISlider slider           {{0, 0},  {300, 0}};
    UIButton reaction_button {{0, 50}, {300, 50}};
public:
    InterfaceView(const Vec2d& position = {0, 0});

    UISlider* get_slider() { return &slider; }
    UIButton* get_reaction_toggle_button() { return &reaction_button; }

    void update_reactions_button(bool reactions_on);
};