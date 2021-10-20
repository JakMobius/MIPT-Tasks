#pragma once

#include "tool.hpp"

class BrushTool : public Tool {
    DrawingContext ctx {};
public:
    BrushTool(): Tool() {}

    void on_mouse_move(Vec2f position) override {
        if(!is_mouse_down) return;
        auto layer = manager->get_canvas()->get_active_layer();
        if(!layer) return;
        auto texture = layer->get_texture();

        ctx.set_color(manager->get_color());
        ctx.push_render_target(texture);
        ctx.draw_circle(position, 3);
        ctx.pop_render_target();

        layer->set_needs_redraw();
    }
};