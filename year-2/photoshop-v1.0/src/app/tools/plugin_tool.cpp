//
// Created by Артем on 29.11.2021.
//

#include "plugin_tool.hpp"
#include "brush/brush_tool.hpp"
#include "../photoshop_view.hpp"

PluginTool::~PluginTool() {
    plugin->tool = nullptr;
}

void PluginTool::on_mouse_down(Vec2f position) {
    Tool::on_mouse_down(position);
    plugin->interface->tool.on_press(AppInterface::Utils::vec2f_to_pvec2f(position));
    old_position = position;
}

void PluginTool::on_mouse_move(Vec2f position) {
    Tool::on_mouse_move(position);
    if(!is_mouse_down) return;
    plugin->interface->tool.on_move(AppInterface::Utils::vec2f_to_pvec2f(old_position),
                                    AppInterface::Utils::vec2f_to_pvec2f(position));
    old_position = position;
}

void PluginTool::on_mouse_up() {
    Tool::on_mouse_up();

    auto layer = AppInterface::Utils::get_current_layer();
    if(layer) {
        UIFillStyleTexture fill_style(layer->get_preview_texture());
        AppInterface::Utils::shared_context.push_render_target(layer->get_texture());
        AppInterface::Utils::shared_context.set_fill_style(&fill_style);
        AppInterface::Utils::shared_context.fill_rect({}, (Vec2f) layer->get_size());
        AppInterface::Utils::shared_context.pop_render_target();
        layer->get_preview_texture()->get_render_texture()->clear({0, 0, 0, 0});
    }

    plugin->interface->tool.on_release(AppInterface::Utils::vec2f_to_pvec2f(old_position));
}

void PluginTool::on_become_active() {
    Tool::on_become_active();
    manager->get_app()->get_plugin_manager()->set_active_plugin_tool(this);
    setup_color_blending();
}

void PluginTool::on_resign_active() {
    Tool::on_resign_active();
    manager->get_app()->get_plugin_manager()->set_active_plugin_tool(nullptr);
}

void PluginTool::setup_color_blending() {
    brush_apply_style.get_render_states()->blendMode = sf::BlendAlpha;
}

