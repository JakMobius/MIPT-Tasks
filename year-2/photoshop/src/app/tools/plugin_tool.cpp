//
// Created by Артем on 29.11.2021.
//

#include "plugin_tool.hpp"
#include "brush/brush_tool.hpp"
#include "../photoshop_view.hpp"
#include "../plugin/app_interface.hpp"

PluginTool::~PluginTool() {
    plugin->tool = nullptr;
}

void PluginTool::on_mouse_down(Vec2f position) {
    Tool::on_mouse_down(position);
    plugin->interface->tool_on_press(HostAppInterface::vec2f_to_pvec2f(position));
    old_position = position;
}

void PluginTool::on_mouse_move(Vec2f position) {
    Tool::on_mouse_move(position);
    if(!is_mouse_down) return;
    plugin->interface->tool_on_move(HostAppInterface::vec2f_to_pvec2f(old_position),
                                    HostAppInterface::vec2f_to_pvec2f(position));
    old_position = position;
}

void PluginTool::on_mouse_up() {
    Tool::on_mouse_up();

    plugin->interface->tool_on_release(HostAppInterface::vec2f_to_pvec2f(old_position));
}

void PluginTool::on_become_active() {
    Tool::on_become_active();
    setup_color_blending();
}

void PluginTool::on_resign_active() {
    Tool::on_resign_active();
}

void PluginTool::setup_color_blending() {
    brush_apply_style.get_render_states()->blendMode = sf::BlendAlpha;
}

