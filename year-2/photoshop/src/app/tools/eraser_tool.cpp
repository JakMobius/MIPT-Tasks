//
// Created by Артем on 21.10.2021.
//

#include "eraser_tool.hpp"

void EraserTool::preserve_color(sf::BlendMode &mode) {
    mode.colorDstFactor = sf::BlendMode::One;
    mode.colorSrcFactor = sf::BlendMode::Zero;
}

void EraserTool::override_color(sf::BlendMode &mode) {
    mode.colorDstFactor = sf::BlendMode::Zero;
    mode.colorSrcFactor = sf::BlendMode::One;
}

void EraserTool::override_alpha(sf::BlendMode &mode) {
    mode.alphaDstFactor = sf::BlendMode::Zero;
    mode.alphaSrcFactor = sf::BlendMode::One;
}

void EraserTool::multiply_alpha(sf::BlendMode &mode) {
    mode.alphaDstFactor = sf::BlendMode::Zero;
    mode.alphaSrcFactor = sf::BlendMode::DstAlpha;
}

EraserTool::EraserTool() : Tool() {
    auto& brush_fill_mode = brush_fill_style.get_render_states()->blendMode;
    override_color(brush_fill_mode);
    override_alpha(brush_fill_mode);

    auto& brush_stroke_mode = brush_stroke_style.get_render_states()->blendMode;
    override_color(brush_stroke_mode);
    override_alpha(brush_stroke_mode);

    auto& texture_copy_mode = texture_copy_style.get_render_states()->blendMode;
    override_color(texture_copy_mode);
    override_alpha(texture_copy_mode);

    auto& apply_mode = brush_apply_style.get_render_states()->blendMode;
    preserve_color(apply_mode);
    multiply_alpha(apply_mode);
}

void EraserTool::draw(Vec2f position) {
    auto layer = manager->get_canvas()->get_active_layer();
    if(!layer) return;
    auto texture = layer->get_texture();

    ctx.push_render_target(map_texture);
    ctx.set_stroke_style(&brush_stroke_style);
    ctx.set_fill_style(&brush_fill_style);
    ctx.fill_circle(position, 3);
    ctx.stroke_line(old_position, position, 6);
    ctx.pop_render_target();

    ctx.push_render_target(texture);
    ctx.clear({0, 0, 0, 0});

    texture_copy_style.set_texture(buffer_texture);
    ctx.set_fill_style(&texture_copy_style);
    ctx.fill_rect({}, (Vec2f) layer->get_size());

    ctx.set_fill_style(&brush_apply_style);
    ctx.fill_rect({}, (Vec2f) layer->get_size());

    ctx.pop_render_target();

    layer->set_needs_redraw();
}

void EraserTool::on_become_active() {
    Tool::on_become_active();

    auto size = manager->get_canvas()->get_active_layer()->get_size();

    map_texture = new DrawingTargetTexture(size);
    buffer_texture = new DrawingTargetTexture(size);

    brush_apply_style.set_texture(map_texture);
}

void EraserTool::on_resign_active() {
    Tool::on_resign_active();

    brush_apply_style.set_texture(nullptr);

    delete map_texture;
    delete buffer_texture;

    buffer_texture = nullptr;
    map_texture = nullptr;
}

void EraserTool::on_mouse_down(Vec2f position) {
    Tool::on_mouse_down(position);

    auto canvas_layer = manager->get_canvas()->get_active_layer();
    if(!canvas_layer) return;
    auto canvas_texture = canvas_layer->get_texture();
    auto canvas_size = canvas_layer->get_size();
    map_texture->clear({1, 1, 1, 1});

    ctx.push_render_target(buffer_texture);
    texture_copy_style.set_texture(canvas_texture);
    ctx.set_fill_style(&texture_copy_style);
    ctx.fill_rect({}, (Vec2f) canvas_size);
    ctx.pop_render_target();

    old_position = position;
    draw(position);
}

void EraserTool::on_mouse_move(Vec2f position) {
    Tool::on_mouse_move(position);
    if(!is_mouse_down) return;
    draw(position);
    old_position = position;
}
