#pragma once

#include "../ui/window/ui_window.hpp"
#include "animation-view.hpp"

class AnimationWindow : public UIWindow {
    AnimationView* m_animation_view = nullptr;

public:
    explicit AnimationWindow(const Vec2f& position): UIWindow(position, {500, 300}, "Sorting Animation") {
        m_animation_view = new AnimationView({10, 10}, {480, 280});
        get_content_view()->append_child(m_animation_view);
    }

    AnimationView* get_animation_view() {
        return m_animation_view;
    }
};