//
// Created by Артем on 20.10.2021.
//

#include "tool_select_window.hpp"
#include "../app.hpp"
#include "color_select_button.hpp"

void ToolSelectWindow::create_tool_buttons() {

    auto& factories = manager->get_factories();

    for(int i = 0; i < factories.size(); i++) {
        auto* factory = manager->get_factories()[i];
        auto* button = new UIButton({}, {250, 50});
        auto* style = new ToolButtonStyle(nullptr);

        button->set_own_style(style);
        stack->append_child(button);

        button->set_text_color({1, 1, 1, 1});
        button->set_text_size(18);
        button->set_title(factory->get_tool_name());

        button->set_callback([this, factory]() {
            manager->activate_factory(factory);
        });
    }

    color_select_button = new ColorSelectButton();
    color_select_button->set_callback([this]() { emit_color_picker(); });
    color_select_button->set_presented_color(manager->get_color());
    stack->append_child(color_select_button);
}

ToolSelectWindow::ToolSelectWindow(PhotoshopView* app, ToolManager* manager, const Vec2f &position) : PhotoshopWindow(app, position, {}, ""), manager(manager) {
    stack = new UIStackView(UIAxis::y);
    stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
    stack->set_item_spacing(7);
    stack->set_insets({7});
    stack->set_fitting({});

    color_listener = [this](ToolManagerColorEvent*){
        color_select_button->set_presented_color(this->manager->get_color());
    };

    manager->get_color_event_emitter()->add_listener(&color_listener);

    get_content_view()->append_child(stack);
    create_tool_buttons();
}

void ToolSelectWindow::layout() {
    stack->layout_if_needed();
    get_content_view()->set_size(stack->get_size());
    UIWindow::layout();
}

void ToolSelectWindow::emit_color_picker() {
    auto container = get_container_view();
    if(!container) return;

    get_app()->open_colorpicker([this](const Vec4f& color) {
        manager->set_color(color);
    }, &manager->get_color());
}

ToolSelectWindow::~ToolSelectWindow() {
    manager->get_color_event_emitter()->remove_listener(&color_listener);
}
