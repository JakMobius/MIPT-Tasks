#pragma once

#include "tool.hpp"
#include "../assets.hpp"

class BrushTool : public Tool {
    DrawingContext ctx {};
    UIFillStyleColor brush_fill_style {{0, 0, 0, 0}};
    UIFillStyleColor brush_stroke_style {{0, 0, 0, 0}};
    Vec2f old_position;
public:
    BrushTool(): Tool() {}

    void on_mouse_down(Vec2f position) override {
        Tool::on_mouse_down(position);
        old_position = position;
        draw(position);
    }

    void on_mouse_move(Vec2f position) override {
        Tool::on_mouse_move(position);
        if(!is_mouse_down) return;
        draw(position);
        old_position = position;
    }

    void draw(Vec2f position) {
        auto layer = manager->get_canvas()->get_active_layer();
        if(!layer) return;
        auto texture = layer->get_texture();

        brush_fill_style.set_color(manager->get_color());
        brush_stroke_style.set_color(manager->get_color());

        ctx.set_fill_style(&brush_fill_style);

        ctx.push_render_target(texture);
        ctx.fill_circle(position, 3);
        ctx.stroke_line(old_position, position, 6);
        ctx.pop_render_target();

        layer->set_needs_redraw();
    }
};