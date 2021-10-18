#pragma once

#include "ui_view.hpp"

enum class UIStackViewDirection {
    x = 0,
    y = 1
};

enum class UIStackViewPrimaryAlignment {
    leading,
    center,
    trailing,
    space_between,
    space_around,
    space_evenly
};

enum class UIStackViewLateralAlignment {
    leading,
    center,
    trailing
};

enum class UIStackViewSpaceMode {
    between,
    around,
    evenly
};

struct UIStackDirFitting {
    bool is_fitting;
    float size;

    UIStackDirFitting(): is_fitting(true), size(0) {};
    UIStackDirFitting(float size): is_fitting(false), size(size) {};
};

struct UIStackViewFitting {
    UIStackDirFitting x {};
    UIStackDirFitting y {};

    UIStackDirFitting& by_direction(UIStackViewDirection direction) {
        if(direction == UIStackViewDirection::x) return x;
        return y;
    }
};

struct UIStackViewInset {
    bool is_relative;
    float inset;

    UIStackViewInset(float inset, bool is_relative = false): inset(inset), is_relative(is_relative) {}

    void set_relative(float fraction) { is_relative = true; inset = fraction; }
    void set_pixels(float pixels) { is_relative = false; inset = pixels; }

    float calculate(float size) const {
        if(is_relative) return size * inset;
        return inset;
    }

    UIStackViewInset(): is_relative(false), inset(0) {}
};

struct UIStackViewInsets {
    UIStackViewInset top, right, bottom, left;

    UIStackViewInsets(UIStackViewInset inset): top(inset), right(inset), bottom(inset), left(inset) {}
    UIStackViewInsets(UIStackViewInset top, UIStackViewInset right, UIStackViewInset bottom, UIStackViewInset left): top(top), right(right), bottom(bottom), left(left) {}
    UIStackViewInsets(UIStackViewInset vertical, UIStackViewInset horizontal): top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    UIStackViewInsets(): top(), right(), bottom(), left() {}

    float get_leading_inset(UIStackViewDirection direction, float size) const {
        if(direction == UIStackViewDirection::x) return left.calculate(size);
        return top.calculate(size);
    }

    float get_trailing_inset(UIStackViewDirection direction, float size) const {
        if(direction == UIStackViewDirection::x) return right.calculate(size);
        return bottom.calculate(size);
    }
};

class UIStackView : public UIView {
    UIStackViewPrimaryAlignment primary_alignment = UIStackViewPrimaryAlignment::leading;
    UIStackViewLateralAlignment lateral_alignment = UIStackViewLateralAlignment::leading;
    UIStackViewInsets insets {};
    UIStackViewFitting fitting {};
    UIStackViewDirection direction = UIStackViewDirection::x;
    float item_spacing = 0;

    void layout_primary_leading(UIStackViewDirection p_direction, float from, float to);
    void layout_primary_trailing(UIStackViewDirection p_direction, float from, float to);
    void layout_primary_center(UIStackViewDirection p_direction, float from, float to);
    void layout_primary_space(UIStackViewDirection p_direction, float from, float to, UIStackViewSpaceMode mode);
    void layout_primary(UIStackViewDirection p_direction, float from, float to);
    void layout_lateral(UIStackViewDirection p_direction, float from, float to);
    float layout_direction(UIStackViewDirection p_direction);

public:
    UIStackView(UIStackViewDirection direction, const Vec2f& position = {}): UIView(position), direction(direction) {};

    void layout() override {
        UIView::layout();

        for(int i = 0; i < children.size; i++) children[i]->layout_if_needed();

        set_size({
            layout_direction(UIStackViewDirection::x),
            layout_direction(UIStackViewDirection::y)
        });
    }

    UIStackViewPrimaryAlignment get_primary_alignment() const { return primary_alignment; }
    void set_primary_alignment(UIStackViewPrimaryAlignment primaryAlignment) { primary_alignment = primaryAlignment; }

    UIStackViewLateralAlignment get_lateral_alignment() const { return lateral_alignment; }
    void set_lateral_alignment(UIStackViewLateralAlignment lateralAlignment) { lateral_alignment = lateralAlignment; }

    const UIStackViewInsets &get_insets() const { return insets; }
    void set_insets(const UIStackViewInsets &p_insets) { insets = p_insets; }

    const UIStackViewFitting &get_fitting() const { return fitting; }
    void set_fitting(const UIStackViewFitting &p_fitting) { fitting = p_fitting; }

    UIStackViewDirection get_direction() const { return direction; }
    void set_direction(UIStackViewDirection p_direction) { direction = p_direction; }

    float get_item_spacing() const { return item_spacing; }
    void set_item_spacing(float itemSpacing) { item_spacing = itemSpacing; }

    float get_primary_size(UIStackViewDirection direction);

    float get_lateral_size(UIStackViewDirection direction);
};