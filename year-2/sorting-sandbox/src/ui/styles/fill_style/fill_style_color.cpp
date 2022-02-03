//
// Created by Артем on 14.11.2021.
//

#include "fill_style_color.hpp"

UIAnimatedFillStyle* UIAnimatedFillStyleColor::get_animated(const UIFillStyle* other) const {

    if(other->get_type() == UIFillStyleType::color) {
        const UIFillStyleColor* other_fill_style_color = (UIFillStyleColor*)(other);
        auto other_color = other_fill_style_color->get_color();
        return new UIAnimatedFillStyleColor(get_current_animation_color(), other_color);
    }

    return nullptr;
}

UIAnimatedFillStyle* UIFillStyleColor::get_animated(const UIFillStyle* other) const {

    if(other->get_type() == UIFillStyleType::color) {
        const UIFillStyleColor* other_fill_style_color = (UIFillStyleColor*)(other);
        auto other_color = other_fill_style_color->get_color();
        return new UIAnimatedFillStyleColor(get_color(), other_color);
    }

    return nullptr;
}
