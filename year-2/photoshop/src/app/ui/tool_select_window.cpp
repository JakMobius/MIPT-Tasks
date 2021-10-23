//
// Created by Артем on 20.10.2021.
//

#include "tool_select_window.hpp"
#include "../app.hpp"

void ToolSelectWindow::create_tool_buttons() {

    auto& factories = manager->get_factories();

    for(int i = 0; i < factories.size(); i++) {
        auto* factory = manager->get_factories()[i];
        auto* button = new UIButton({}, {50, 50});
        auto* fill_style = new UIFillStyleTexture(factories[i]->get_tool_icon());
        auto* style = new ToolButtonStyle(fill_style);

        button->set_own_style(style);
        stack->append_child(button);

        fill_style->set_scale((Vec2f) factory->get_tool_icon()->get_size() / button->get_size());

        button->set_callback([this, factory]() {
            manager->activate_factory(factory);
        });
    }

    auto* color_button = new UIButton({}, {100, 50});
    color_button->set_callback([this]() {
        emit_color_picker();
    });

    stack->append_child(color_button);
}

ToolSelectWindow::ToolSelectWindow(PhotoshopView* app, ToolManager* manager, const Vec2f &position) : PhotoshopWindow(app, position, {}, ""), manager(manager) {
    stack = new UIStackView(UIStackViewDirection::y);
    stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
    stack->set_item_spacing(7);
    stack->set_insets({7});
    stack->set_fitting({});

    stack->set_fill_style(&UIViewWhiteBackground);

    get_content_view()->append_child(stack);
    create_tool_buttons();
}

void ToolSelectWindow::layout() {
    stack->layout_if_needed();
    get_content_view()->set_size(stack->get_size());
    UIWindow::layout();
}
