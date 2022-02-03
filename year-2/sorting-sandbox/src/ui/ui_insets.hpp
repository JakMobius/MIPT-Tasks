#pragma once

struct UIInset;

#include "ui_axis.hpp"
#include "../utils/vec2.hpp"

struct UIInset {
    bool is_relative;
    float inset;

    UIInset(float inset, bool is_relative = false): inset(inset), is_relative(is_relative) {}

    void set_relative(float fraction) { is_relative = true; inset = fraction; }
    void set_pixels(float pixels) { is_relative = false; inset = pixels; }

    float calculate(float size) const {
        if(is_relative) return size * inset;
        return inset;
    }

    UIInset(): is_relative(false), inset(0) {}

    bool operator==(const UIInset& other) const {
        return is_relative == other.is_relative && inset == other.inset;
    }
};


struct UIInsets {
    UIInset top, right, bottom, left;

    UIInsets(UIInset inset): top(inset), right(inset), bottom(inset), left(inset) {}
    UIInsets(UIInset top, UIInset right, UIInset bottom, UIInset left): top(top), right(right), bottom(bottom), left(left) {}
    UIInsets(UIInset vertical, UIInset horizontal): top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    UIInsets(): top(), right(), bottom(), left() {}

    float get_leading_inset(UIAxis direction, float size) const;
    float get_trailing_inset(UIAxis direction, float size) const;
    Vec2f get_nested_size(Vec2f size) const;
    Vec2f get_nested_position(Vec2f size) const;

    bool operator==(const UIInsets& other) const {
        return top == other.top && right == other.right && bottom == other.bottom && left == other.left;
    }
};
