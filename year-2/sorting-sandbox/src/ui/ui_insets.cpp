//
// Created by Артем on 07.11.2021.
//

#include "ui_insets.hpp"

float UIInsets::get_leading_inset(UIAxis direction, float size) const {
    if(direction == UIAxis::x) return left.calculate(size);
    return top.calculate(size);
}

float UIInsets::get_trailing_inset(UIAxis direction, float size) const {
    if(direction == UIAxis::x) return right.calculate(size);
    return bottom.calculate(size);
}

Vec2f UIInsets::get_nested_position(Vec2f size) const {
    return {left.calculate(size[0]), top.calculate(size[1])};
}


Vec2f UIInsets::get_nested_size(Vec2f size) const {
    size.set_x(size[0] - left.calculate(size[0]) - right.calculate(size[0]));
    size.set_y(size[1] - top.calculate(size[1]) - bottom.calculate(size[1]));
    return size;
}
