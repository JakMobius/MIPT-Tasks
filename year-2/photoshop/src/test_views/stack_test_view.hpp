#pragma once

#include "../ui/ui_view.hpp"
#include "../ui/ui_stack.hpp"
#include "../ui/ui_slider.hpp"
#include "../ui/ui_text.hpp"
#include "../ui/ui_button.hpp"
#include <vector>

class LayoutTestView : public UIView {

    float spacing = 0;
    UIInsets stack_insets {};
    std::vector<UIStackView*> stacks;
    std::vector<UIStackView*> containers;
    UIAxis direction = UIAxis::x;
    UIStackViewPrimaryAlignment primary_alignment = UIStackViewPrimaryAlignment::leading;
    UIStackViewFitting fitting = {{350}, {700}};
    UIStackView* controls;

    void layout() override;
    void create_layout(const Vec2f& position, UIStackViewLateralAlignment lateral);
    void create_controls(const Vec2f& position);
    void setup_inset_slider(const char* text, UIInset* target);
    void add_inset_sliders();
    void add_spacing_slider();
    void add_primary_alignment_buttons();
    void add_h_v_modes_buttons();
    void add_fit_modes_buttons();

public:
    LayoutTestView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}) : UIView(position, size) {

        create_layout({30, 0}, UIStackViewLateralAlignment::leading);
        create_layout({420, 0}, UIStackViewLateralAlignment::center);
        create_layout({810, 0}, UIStackViewLateralAlignment::trailing);

        create_controls({1200, 0});
    }
};