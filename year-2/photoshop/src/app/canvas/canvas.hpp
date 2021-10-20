#pragma once

#include "../../graphics/drawing_context.hpp"
#include "../../events/event_emitter.hpp"
#include "canvas_layer.hpp"

class Canvas;
struct CanvasUpdateEvent { Canvas* canvas; };

class LayerUpdateEventHandler : public EventHandler<LayerUpdateEvent> {
    Canvas* canvas;
public:
    LayerUpdateEventHandler(Canvas* canvas): canvas(canvas) {}
    void operator() (LayerUpdateEvent* event) override;
};

class Canvas {
    DrawingContext* canvas_ctx = nullptr;
    CanvasLayer* active_layer = nullptr;
    std::vector<CanvasLayer*> layers {};
    Vec2i size;

    LayerUpdateEventHandler handler { this };
    EventEmitter<CanvasUpdateEvent> canvas_update_event_emitter {};

public:
    Canvas(const Vec2i& size): size(size) {
        canvas_ctx = new DrawingContext();
    }

    ~Canvas() {
        delete canvas_ctx;
    }

    void draw(DrawingContext* ctx);

    EventEmitter<CanvasUpdateEvent>* get_event_emitter() { return &canvas_update_event_emitter; }

    const std::vector<CanvasLayer*>& get_layers() { return layers; }
    void add_layer(CanvasLayer* p_layer);
    void remove_layer(CanvasLayer* p_layer);
    CanvasLayer* get_active_layer() const { return active_layer; }
    void set_active_layer(CanvasLayer* p_active_layer) { active_layer = p_active_layer; }

    void on_layer_updated();
};