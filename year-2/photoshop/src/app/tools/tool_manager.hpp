#pragma once

class ToolManager;

#include "tool.hpp"
#include "../../utils/vec4.hpp"
#include "../canvas/canvas.hpp"

class ToolManager {
    Tool* active_tool = nullptr;
    Vec4f selected_color = {1, 1, 1, 1};
    Canvas* canvas;
public:
    explicit ToolManager(Canvas* canvas): canvas(canvas) {}

    void on_mouse_down(Vec2f position);
    void on_mouse_move(Vec2f position);
    void on_mouse_up();

    Canvas* get_canvas() { return canvas; }
    void set_active_tool(Tool* p_active_tool);
    Tool* get_active_tool() const { return active_tool; }
    void set_color(const Vec4f& p_selected_color) { selected_color = p_selected_color; }
    Vec4f get_color() const { return selected_color; }
};