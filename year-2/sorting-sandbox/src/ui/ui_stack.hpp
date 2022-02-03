#pragma once

#include "ui_view.hpp"
#include "ui_insets.hpp"
#include "ui_axis.hpp"

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

    bool operator==(const UIStackDirFitting& other) const {
        return is_fitting == other.is_fitting && size == other.size;
    }
};

struct UIStackViewFitting {
    UIStackDirFitting x {};
    UIStackDirFitting y {};

    UIStackDirFitting& by_direction(UIAxis direction) {
        if(direction == UIAxis::x) return x;
        return y;
    }

    bool operator==(const UIStackViewFitting& other) const {
        return x == other.x && y == other.y;
    }
};

class UIStackView : public UIView {
    UIStackViewPrimaryAlignment primary_alignment = UIStackViewPrimaryAlignment::leading;
    UIStackViewLateralAlignment lateral_alignment = UIStackViewLateralAlignment::leading;
    UIInsets insets {};
    UIStackViewFitting fitting {};
    UIAxis direction = UIAxis::x;
    float item_spacing = 0;

    void layout_primary_leading(UIAxis p_direction, float from, float to);
    void layout_primary_trailing(UIAxis p_direction, float from, float to);
    void layout_primary_center(UIAxis p_direction, float from, float to);
    void layout_primary_space(UIAxis p_direction, float from, float to, UIStackViewSpaceMode mode);
    void layout_primary(UIAxis p_direction, float from, float to);
    void layout_lateral(UIAxis p_direction, float from, float to);
    float layout_direction(UIAxis p_direction);

public:
    UIStackView(UIAxis direction, const Vec2f& position = {}): UIView(position), direction(direction) {};

    void layout() override {
        UIView::layout();

        if(get_needs_layout()) {
            set_size({
                 layout_direction(UIAxis::x),
                 layout_direction(UIAxis::y)
            });
        }
    }

    UIStackViewPrimaryAlignment get_primary_alignment() const { return primary_alignment; }
    void set_primary_alignment(UIStackViewPrimaryAlignment primaryAlignment) { primary_alignment = primaryAlignment; set_needs_layout(); }

    UIStackViewLateralAlignment get_lateral_alignment() const { return lateral_alignment; }
    void set_lateral_alignment(UIStackViewLateralAlignment lateralAlignment) { lateral_alignment = lateralAlignment; set_needs_layout(); }

    const UIInsets &get_insets() const { return insets; }
    void set_insets(const UIInsets &p_insets) {
        if(insets == p_insets) return;
        insets = p_insets;
        set_needs_layout();
    }

    const UIStackViewFitting &get_fitting() const { return fitting; }
    void set_fitting(const UIStackViewFitting &p_fitting) {
        if(fitting == p_fitting) return;
        fitting = p_fitting;
        set_needs_layout();
    }

    UIAxis get_direction() const { return direction; }
    void set_direction(UIAxis p_direction) { direction = p_direction; set_needs_layout(); }

    float get_item_spacing() const { return item_spacing; }
    void set_item_spacing(float itemSpacing) { item_spacing = itemSpacing; set_needs_layout(); }

    float get_primary_size(UIAxis direction);

    float get_lateral_size(UIAxis direction);
};