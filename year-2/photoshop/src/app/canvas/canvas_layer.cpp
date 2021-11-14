//
// Created by Артем on 20.10.2021.
//

#include "canvas_layer.hpp"

void CanvasLayer::set_needs_redraw() {
    needs_redraw = true;
    LayerUpdateEvent event { this };
    layer_update_event_emitter.emit(&event);
}

CanvasLayer::~CanvasLayer() {
    delete preferences_generator;
    delete texture;
}

CanvasLayer::CanvasLayer(Vec2i size) : size(size) {
    texture = new DrawingTargetTexture(size);
    texture->clear({0, 0, 0, 0});
    draw_style.set_texture(texture);
}

void CanvasLayer::draw(Canvas* canvas, DrawingContext* ctx) {
    if(needs_clear) clear_texture();

    needs_redraw = false;

    Vec2f texture_size = { (float)size[0], (float)size[1] };
    Vec2f texture_position = { 0, 0 };
    ctx->set_fill_style(&draw_style);
    ctx->fill_rect(texture_position, texture_size);
}

void CanvasLayer::clear_texture() {
    texture->clear({0, 0, 0, 0});
    needs_clear = false;
}

