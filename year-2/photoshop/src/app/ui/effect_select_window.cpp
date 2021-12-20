
//
// Created by Артем on 01.12.2021.
//

#include "effect_select_window.hpp"
#include "../app.hpp"
#include "../plugin/app_interface.hpp"

void EffectSelectWindow::create_tool_buttons() {

    auto& plugins = app->get_plugin_manager()->get_plugins();

    for(auto plugin : plugins) {
        if(!plugin->is_effect) continue;

        auto* button = new UIButton({}, {250, 50});

        stack->append_child(button);

        button->set_text_size(18);
        button->set_title(plugin->interface->get_info()->name);

        button->set_callback([this, plugin]() {
            if(plugin->host_interface->get_target()) {
                plugin->interface->effect_apply();
            }
        });
    }
}

EffectSelectWindow::EffectSelectWindow(PhotoshopView* app, const Vec2f &position) : PhotoshopWindow(app, position, {}, "") {
    stack = new UIStackView(UIAxis::y);
    stack->set_primary_alignment(UIStackViewPrimaryAlignment::leading);
    stack->set_item_spacing(7);
    stack->set_insets({7});
    stack->set_fitting({});

    set_title("Effects");

    get_content_view()->append_child(stack);
    create_tool_buttons();
}

void EffectSelectWindow::layout() {
    stack->layout_if_needed();
    get_content_view()->set_size(stack->get_size());
    UIWindow::layout();
}
