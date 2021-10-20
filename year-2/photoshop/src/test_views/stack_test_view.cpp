//
// Created by Артем on 17.10.2021.
//

#include "stack_test_view.hpp"
#include "../ui/select/ui_select.hpp"

const UIFillStyleColor test_colors[] = {
    UIFillStyleColor({1, 0, 0, 1}),
    UIFillStyleColor({0, 1, 0, 1}),
    UIFillStyleColor({0, 0, 1, 1}),
    UIFillStyleColor({1, 1, 0, 1}),
    UIFillStyleColor({1, 0, 1, 1}),
};

void LayoutTestView::layout() {
    for(int i = 0; i < stacks.size(); i++) {
        auto stack = stacks[i];
        stack->set_fitting(fitting);
        stack->set_fill_style(&UIViewWhiteBackground);
        stack->set_insets(stack_insets);
        stack->set_item_spacing(spacing);
        stack->set_primary_alignment(primary_alignment);
        stack->set_direction(direction);
        stack->set_needs_layout();
    }
}

void LayoutTestView::create_layout(const Vec2f &position, UIStackViewLateralAlignment lateral) {
    auto container = new UIStackView(UIStackViewDirection::x, position);
    container->set_fitting({350, 1200});
    container->set_primary_alignment(UIStackViewPrimaryAlignment::center);
    container->set_lateral_alignment(UIStackViewLateralAlignment::center);

    auto stack = new UIStackView(UIStackViewDirection::x, {0, 0});
    stack->set_lateral_alignment(lateral);

    container->append_child(stack);
    append_child(container);

    stacks.push_back(stack);
    containers.push_back(container);

    auto* view_a = new UIView();
    auto* view_b = new UIView();
    auto* view_c = new UIView();

    view_a->set_size({60, 250});
    view_b->set_size({30, 200});
    view_c->set_size({120, 100});

    view_a->set_fill_style(&test_colors[0]);
    view_b->set_fill_style(&test_colors[1]);
    view_c->set_fill_style(&test_colors[2]);

    stack->append_child(view_a);
    stack->append_child(view_b);
    stack->append_child(view_c);
}

void LayoutTestView::create_controls(const Vec2f &position) {
    auto controls_container = new UIStackView(UIStackViewDirection::y, position);
    controls_container->set_fitting({300, 1200});
    controls_container->set_lateral_alignment(UIStackViewLateralAlignment::center);
    controls_container->set_primary_alignment(UIStackViewPrimaryAlignment::center);

    controls = new UIStackView(UIStackViewDirection::y);
    controls->set_fill_style(&UIViewWhiteBackground);
    controls->set_lateral_alignment(UIStackViewLateralAlignment::center);
    controls->set_insets({25});

    controls_container->append_child(controls);
    append_child(controls_container);

    add_inset_sliders();
    add_spacing_slider();
    add_primary_alignment_buttons();
    add_h_v_modes_buttons();
    add_fit_modes_buttons();

    auto* select = new UISelectView();
    select->add_variant({ "Option 1" });
    select->add_variant({ "Option 2" });
    select->add_variant({ "Option 3" });
    select->add_variant({ "Option 4" });
    controls->append_child(select);
}

void LayoutTestView::setup_inset_slider(const char* text, UIStackViewInset* target) {

    auto container = new UIStackView(UIStackViewDirection::x);

    auto text_view = new UIText({}, {50, 50});
    text_view->set_text(text);
    text_view->set_v_text_alignment(VTextAlignmentCenter);
    text_view->set_h_text_alignment(HTextAlignmentCenter);

    auto slider = new UISlider({}, {225, 30});
    slider->set_fraction(0);

    slider->set_callback([this, target](float fraction) -> void {
        target->set_pixels(fraction * 30);
        set_needs_layout();
    });

    container->append_child(text_view);
    container->append_child(slider);
    controls->append_child(container);
}

void LayoutTestView::add_inset_sliders() {
    auto text_view = new UIText({}, {250, 50}, "Insets:");
    controls->append_child(text_view);

    setup_inset_slider("t", &stack_insets.top);
    setup_inset_slider("r", &stack_insets.right);
    setup_inset_slider("b", &stack_insets.bottom);
    setup_inset_slider("l", &stack_insets.left);

    controls->append_child(new UIView({}, {25, 25}));
}

void LayoutTestView::add_spacing_slider() {
    auto text_view = new UIText({}, {250, 50});
    text_view->set_text("Spacing:");
    controls->append_child(text_view);

    auto spacing_slider = new UISlider({}, {250, 0});
    controls->append_child(spacing_slider);
    spacing_slider->set_fraction(0);
    spacing_slider->set_callback([this](float fraction) -> void {
        spacing = fraction * 30;
        set_needs_layout();
    });

    controls->append_child(new UIView({}, {25, 25}));
}

void LayoutTestView::add_primary_alignment_buttons() {
    const char* modes[] = {
            "leading",
            "center",
            "trailing",
            "space_between",
            "space_around",
            "space_evenly"
    };

    auto buttons = new std::vector<UIButton*>();

    for(int i = 0; i < 6; i++) {
        auto button = new UIButton({}, {250, 50});
        button->set_selected(i == 0);
        buttons->push_back(button);
        button->set_title(modes[i]);
        controls->append_child(button);
        button->set_callback([this, i, buttons]() -> void {
            primary_alignment = (UIStackViewPrimaryAlignment) i;
            for(int j = 0; j < buttons->size(); j++) (*buttons)[j]->set_selected(i == j);
            set_needs_layout();
        });
    }

    controls->append_child(new UIView({}, {25, 25}));
}

void LayoutTestView::add_h_v_modes_buttons() {
    const char* directions[] = {
            "horizontal",
            "vertical"
    };

    auto buttons = new std::vector<UIButton*>();

    for(int i = 0; i < 2; i++) {
        auto button = new UIButton({}, {250, 50});
        button->set_selected(i == 0);
        buttons->push_back(button);
        button->set_title(directions[i]);
        button->set_callback([this, i, buttons]() -> void {
            direction = (UIStackViewDirection) i;
            for(int j = 0; j < buttons->size(); j++) (*buttons)[j]->set_selected(i == j);
            set_needs_layout();
        });
        controls->append_child(button);
    }

    controls->append_child(new UIView({}, {25, 25}));
}

void LayoutTestView::add_fit_modes_buttons() {
    const char* fit_modes[] = {
            "fixed size",
            "fit"
    };

    auto buttons = new std::vector<UIButton*>();

    for(int i = 0; i < 2; i++) {
        auto button = new UIButton({}, {250, 50});
        button->set_selected(i == 0);
        buttons->push_back(button);
        button->set_title(fit_modes[i]);
        button->set_callback([this, i, buttons]() -> void {
            if(i == 0) fitting = {350, 700};
            else fitting = {{} , {}};
            for(int j = 0; j < buttons->size(); j++) (*buttons)[j]->set_selected(i == j);
            set_needs_layout();
        });
        controls->append_child(button);
    }

    controls->append_child(new UIView({}, {25, 25}));
}
