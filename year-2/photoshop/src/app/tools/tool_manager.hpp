#pragma once

class ToolManager;

struct ToolManagerColorEvent {
    ToolManager* manager;
};

struct CanvasChangeEvent {
    ToolManager* manager;
};

#include "tool.hpp"
#include "../../utils/vec4.hpp"
#include "../canvas/canvas.hpp"
#include "factory/tool_factory.hpp"

class ToolManager {
    Tool* active_tool = nullptr;
    Vec4f selected_color = {1, 1, 1, 1};
    Canvas* canvas = nullptr;
    EventEmitter<ToolManagerColorEvent> color_event_emitter;
    EventEmitter<CanvasChangeEvent> canvas_change_emitter;
    std::vector<ToolFactoryBase*> tool_factories;

    EventHandler<ActiveLayerChangeEvent> active_layer_change_handler {[this](ActiveLayerChangeEvent* event) {
        if(active_tool) active_tool->on_layer_change();
    }};

public:
    explicit ToolManager() {}

    void on_mouse_down(Vec2f position);
    void on_mouse_move(Vec2f position);
    void on_mouse_up();

    void set_active_canvas(Canvas* p_canvas);
    Canvas* get_active_canvas() { return canvas; }
    void activate_factory(ToolFactoryBase* p_active_tool);
    Tool* get_active_tool() const { return active_tool; }
    void set_color(const Vec4f& p_selected_color);
    const Vec4f& get_color() const { return selected_color; }

    void add_tool_factory(ToolFactoryBase* factory) { tool_factories.push_back(factory); }
    const std::vector<ToolFactoryBase*>& get_factories() const { return tool_factories; }

    EventEmitter<ToolManagerColorEvent>* get_color_event_emitter() { return &color_event_emitter; }
    EventEmitter<CanvasChangeEvent>* get_canvas_change_event_emitter() { return &canvas_change_emitter; }
};