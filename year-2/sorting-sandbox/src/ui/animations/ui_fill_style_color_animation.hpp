#pragma once

#include "../ui_view.hpp"
#include "ui_animation.hpp"

class UIFillStyleAnimation : public UIAnimation {
    UIView* target;
    UIAnimatedFillStyle* style = nullptr;
    const UIFillStyle* target_style = nullptr;
    float duration;
    float time = 0;

    EventHandler<ViewDestroyEvent> view_destroy_handler {[this](ViewDestroyEvent*) {
        delete style;
        style = nullptr;
        target = nullptr;
    }};

    void setup_animated_style(const UIFillStyle* new_fill_style);

public:
    UIFillStyleAnimation(UIView* target, const UIFillStyle* new_style, float duration);

    bool tick(float dt) override;

    void end();
};