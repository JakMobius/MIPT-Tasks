//
// Created by Артем on 20.10.2021.
//

#include "tool_manager.hpp"

void ToolManager::activate_factory(ToolFactoryBase* factory) {
    if(active_tool) {
        active_tool->on_resign_active();
        delete active_tool;
    }
    if(factory) {
        active_tool = factory->create_tool();
        active_tool->set_manager(this),
        active_tool->on_become_active();
    }
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

void ToolManager::set_active_canvas(Canvas* p_canvas) {
    if(canvas == p_canvas) return;
    if(canvas != nullptr) canvas->get_active_layer_change_event_emitter()->remove_listener(&active_layer_change_handler);
    canvas = p_canvas;
    if(canvas) canvas->get_active_layer_change_event_emitter()->add_listener(&active_layer_change_handler);
    CanvasChangeEvent event { this };
    canvas_change_emitter.emit(&event);
    if(active_tool) active_tool->on_layer_change();
}

void ToolManager::set_color(const Vec4f &p_selected_color) {
    selected_color = p_selected_color;
    ToolManagerColorEvent event { this };
    color_event_emitter.emit(&event);
}
