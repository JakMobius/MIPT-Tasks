#pragma once

class CanvasLayer;

#include "../../graphics/drawing_target_texture.hpp"
#include "../../graphics/drawing_context.hpp"
#include "../../events/event_emitter.hpp"

struct LayerUpdateEvent {
    CanvasLayer* layer;
};

class CanvasLayer {
    EventEmitter<LayerUpdateEvent> layer_update_event_emitter {};
    DrawingTargetTexture* texture = nullptr;
    Vec2i size;
    bool needs_redraw = false;

public:
    explicit CanvasLayer(Vec2i size): size(size) {
        texture = new DrawingTargetTexture(size);
        texture->clear({0, 0, 0, 0});
    }

    void draw(DrawingContext* ctx) {
        needs_redraw = false;
        Vec2f texture_size = { (float)size[0], (float)-size[1] };
        Vec2f texture_position = { 0, (float)size[1] };
        ctx->draw_texture(texture_position, texture_size, texture);
    }

    void set_needs_redraw();
    bool get_needs_redraw() const { return needs_redraw; }
    EventEmitter<LayerUpdateEvent>* get_layer_update_event_emitter() { return &layer_update_event_emitter; }

    DrawingTargetTexture* get_texture() { return texture; }

    Vec2i get_size() const { return size; }
};