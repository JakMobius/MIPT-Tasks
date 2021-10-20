//
// Created by Артем on 20.10.2021.
//

#include "tool_manager.hpp"

void ToolManager::set_active_tool(Tool* p_active_tool) {
    if(active_tool) active_tool->on_resign_active();
    active_tool = p_active_tool;
    active_tool->set_manager(this),
    active_tool->on_become_active();
}

void ToolManager::on_mouse_down(Vec2f position) {
    if(active_tool) active_tool->on_mouse_down(position);
}

void ToolManager::on_mouse_move(Vec2f position) {
    if(active_tool) active_tool->on_mouse_move(position);
}

void ToolManager::on_mouse_up() {
    if(active_tool) active_tool->on_mouse_up();
}
