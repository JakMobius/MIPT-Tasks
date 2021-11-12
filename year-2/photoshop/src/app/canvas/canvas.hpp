#pragma once

class Canvas;
struct CanvasUpdateEvent { Canvas* canvas; };

#include "../../graphics/drawing_context.hpp"
#include "../../events/event_emitter.hpp"
#include "canvas_layer.hpp"

class Canvas {
    CanvasLayer* active_layer = nullptr;
    DrawingTargetTexture* frame_buffer = nullptr;
    std::vector<CanvasLayer*> layers {};
    Vec2i size;
    bool needs_redraw = false;

    UIFillStyleTexture frame_buffer_style {};

    EventHandler<LayerUpdateEvent> handler {[this](LayerUpdateEvent* event) { on_layer_updated(); }};
    EventEmitter<CanvasUpdateEvent> canvas_update_event_emitter {};

public:
    Canvas(const Vec2i& size): size(size) {
        frame_buffer = new DrawingTargetTexture(size);
        frame_buffer_style.set_texture(frame_buffer);
    }

    ~Canvas() {
        for(int i = 0; i < layers.size(); i++) delete layers[i];
        delete frame_buffer;
    }

    void draw(DrawingContext* ctx);

    EventEmitter<CanvasUpdateEvent>* get_event_emitter() { return &canvas_update_event_emitter; }

    const std::vector<CanvasLayer*>& get_layers() { return layers; }
    void add_layer(CanvasLayer* p_layer);
    void remove_layer(CanvasLayer* layer);
    CanvasLayer* get_active_layer() const { return active_layer; }
    void set_active_layer(CanvasLayer* p_active_layer) { active_layer = p_active_layer; on_layer_updated(); }

    void on_layer_updated();

    const Vec2i& get_size() const { return size; }

    DrawingTargetTexture* get_framebuffer() { return frame_buffer; };

    void set_framebuffer(DrawingTargetTexture* p_frame_buffer) {
        frame_buffer = p_frame_buffer;
    }
};