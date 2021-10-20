#pragma once

#include "tool.hpp"

class BrushTool : public Tool {
    DrawingContext ctx {};
    UIFillStyleColor brush_fill_style {{0, 0, 0, 0}};
public:
    BrushTool(): Tool() {}

    void on_mouse_move(Vec2f position) override {
        if(!is_mouse_down) return;
        auto layer = manager->get_canvas()->get_active_layer();
        if(!layer) return;
        auto texture = layer->get_texture();

        brush_fill_style.set_color(manager->get_color());
        ctx.set_fill_style(&brush_fill_style);
        ctx.push_render_target(texture);
        ctx.fill_circle(position, 3);
        ctx.pop_render_target();

        layer->set_needs_redraw();
    }
};