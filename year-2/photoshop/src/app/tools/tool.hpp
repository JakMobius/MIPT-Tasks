#pragma once

class Tool;

#include "tool_manager.hpp"
#include "../../utils/vec2.hpp"
#include "../../graphics/drawable_texture.hpp"

class Tool {
protected:
    ToolManager* manager = nullptr;
    bool is_mouse_down = false;
public:
    Tool() {}
    virtual ~Tool() {}

    void set_manager(ToolManager* p_manager) { manager = p_manager; };
    ToolManager* get_manager() { return manager; };

    virtual void on_mouse_down(Vec2f position) { is_mouse_down = true; }
    virtual void on_mouse_move(Vec2f position) {}
    virtual void on_mouse_up() { is_mouse_down = false; }
    virtual void on_become_active() {}
    virtual void on_resign_active() {}
};