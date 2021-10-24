#pragma once

class ToolManager;

#include "tool.hpp"
#include "../../utils/vec4.hpp"
#include "../canvas/canvas.hpp"
#include "factory/tool_factory.hpp"

struct ToolManagerColorEvent {
    ToolManager* manager;
};

class ToolManager {
    Tool* active_tool = nullptr;
    Vec4f selected_color = {1, 1, 1, 1};
    Canvas* canvas;
    EventEmitter<ToolManagerColorEvent> color_event_emitter;
    std::vector<ToolFactoryBase*> tool_factories;
public:
    explicit ToolManager(Canvas* canvas): canvas(canvas) {}

    void on_mouse_down(Vec2f position);
    void on_mouse_move(Vec2f position);
    void on_mouse_up();

    Canvas* get_canvas() { return canvas; }
    void activate_factory(ToolFactoryBase* p_active_tool);
    Tool* get_active_tool() const { return active_tool; }
    void set_color(const Vec4f& p_selected_color) {
        selected_color = p_selected_color;
        ToolManagerColorEvent event { this };
        color_event_emitter.emit(&event);
    }
    const Vec4f& get_color() const { return selected_color; }

    void add_tool_factory(ToolFactoryBase* factory) { tool_factories.push_back(factory); }
    const std::vector<ToolFactoryBase*>& get_factories() const { return tool_factories; }

    EventEmitter<ToolManagerColorEvent>* get_color_event_emitter() { return &color_event_emitter; }
};