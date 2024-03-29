//
// Created by Артем on 20.10.2021.
//

#include "brush_tool.hpp"

void BrushTool::preserve_color(sf::BlendMode &mode) {
    mode.colorDstFactor = sf::BlendMode::One;
    mode.colorSrcFactor = sf::BlendMode::Zero;
}

void BrushTool::override_color(sf::BlendMode &mode) {
    mode.colorDstFactor = sf::BlendMode::Zero;
    mode.colorSrcFactor = sf::BlendMode::One;
}

void BrushTool::blend_color(sf::BlendMode &mode) {
    mode.colorDstFactor = sf::BlendMode::OneMinusSrcAlpha;
    mode.colorSrcFactor = sf::BlendMode::SrcAlpha;
}

void BrushTool::override_alpha(sf::BlendMode &mode) {
    mode.alphaDstFactor = sf::BlendMode::Zero;
    mode.alphaSrcFactor = sf::BlendMode::One;
}

void BrushTool::multiply_alpha(sf::BlendMode &mode) {
    mode.alphaDstFactor = sf::BlendMode::Zero;
    mode.alphaSrcFactor = sf::BlendMode::DstAlpha;
}

void BrushTool::blend_alpha(sf::BlendMode &mode) {
    mode.alphaDstFactor = sf::BlendMode::OneMinusSrcAlpha;
    mode.alphaSrcFactor = sf::BlendMode::One;
}

BrushTool::BrushTool() : Tool() {}

void BrushTool::draw(Vec2f position) {
    auto canvas = manager->get_active_canvas();
    if(!canvas) return;

    auto layer = canvas->get_active_layer();
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

void BrushTool::on_become_active() {
    Tool::on_become_active();
    setup_color_blending();
    create_textures();
}

void BrushTool::on_resign_active() {
    Tool::on_resign_active();
    delete_textures();
}

void BrushTool::create_textures() {
    auto canvas = manager->get_active_canvas();
    if(!canvas) return;

    auto active_layer = canvas->get_active_layer();
    if(!active_layer) return;

    auto size = active_layer->get_size();

    map_texture = new DrawingTargetTexture(size);
    buffer_texture = new DrawingTargetTexture(size);

    brush_apply_style.set_texture(map_texture);
}

void BrushTool::delete_textures() {
    brush_apply_style.set_texture(nullptr);

    delete map_texture;
    delete buffer_texture;

    buffer_texture = nullptr;
    map_texture = nullptr;
}

void BrushTool::on_mouse_down(Vec2f position) {
    Tool::on_mouse_down(position);

    auto canvas = manager->get_active_canvas();
    if(!canvas) return;

    auto canvas_layer = canvas->get_active_layer();
    if(!canvas_layer) return;

    auto canvas_texture = canvas_layer->get_texture();
    auto canvas_size = canvas_layer->get_size();

    map_texture->clear({0, 0, 0, 0});

    ctx.push_render_target(buffer_texture);
    texture_copy_style.set_texture(canvas_texture);
    ctx.set_fill_style(&texture_copy_style);
    ctx.fill_rect({}, (Vec2f) canvas_size);
    ctx.pop_render_target();

    prepare_to_draw();

    old_position = position;
    draw(position);
}

void BrushTool::prepare_to_draw() {
    Vec4f brush_color(manager->get_color());
    brush_color.set_w(1);
    brush_fill_style.set_color(brush_color);
    brush_stroke_style.set_color(brush_color);
    brush_apply_style.set_color({1, 1, 1, manager->get_color()[3]});
}

void BrushTool::on_mouse_move(Vec2f position) {
    Tool::on_mouse_move(position);
    if(!is_mouse_down) return;
    draw(position);
    old_position = position;
}

void BrushTool::setup_color_blending() {
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
    blend_color(apply_mode);
    blend_alpha(apply_mode);
}

void BrushTool::on_layer_change() {
    Tool::on_layer_change();
    delete_textures();
    create_textures();
}

BrushTool::~BrushTool() {
    delete_textures();
}