//
// Created by Артем on 14.11.2021.
//

#include "ui_fill_style_color_animation.hpp"

UIFillStyleAnimation::UIFillStyleAnimation(UIView* target, const UIFillStyle* new_style, float duration): target(target), duration(duration) {
    setup_animated_style(new_style);
    if(!style) target->set_fill_style(new_style);
    else {
        target_style = new_style;
        target->set_fill_style(style);
        target->get_destroy_event_emitter()->add_listener(&view_destroy_handler);
    }
}

bool UIFillStyleAnimation::tick(float dt) {
    if(!style || !target) return false;
    if(target->get_fill_style() != style) {
        end();
        return false;
    }

    time += dt;

    float fraction = time / duration;
    bool over = fraction > 1;

    if(over) {
        end();
        return false;
    }

    style->set_animation_fraction(fraction);
    target->set_needs_redraw();
    return true;
}

void UIFillStyleAnimation::setup_animated_style(const UIFillStyle* new_fill_style) {
    const UIFillStyle* old_fill_style = target->get_fill_style();
    if(!old_fill_style) return;

    style = old_fill_style->get_animated(new_fill_style);
}

void UIFillStyleAnimation::end() {
    if(!style || !target) return;

    if(target->get_fill_style() == style) target->set_fill_style(target_style);
    target->get_destroy_event_emitter()->remove_listener(&view_destroy_handler);

    delete style;
    style = nullptr;
    target = nullptr;
}
