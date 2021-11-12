//
// Created by Артем on 20.10.2021.
//

#include "canvas.hpp"

void Canvas::remove_layer(CanvasLayer* layer) {
    if(layer == active_layer) active_layer = nullptr;
    layer->get_layer_update_event_emitter()->remove_listener(&handler);
    for(int i = 0; i < layers.size(); i++) {
        if(layers[i] == layer) {
            layers.erase(layers.begin() + i);
            break;
        }
    }
    on_layer_updated();
}

void Canvas::draw(DrawingContext* ctx) {
    if(needs_redraw) {
        ctx->push_render_target(frame_buffer);
        ctx->clear({0, 0, 0, 0});
        auto saved_transform = ctx->transform;
        ctx->transform = {};

        for(int i = 0; i < layers.size(); i++) {
            layers[i]->draw(this, ctx);
        }

        ctx->transform = saved_transform;
        ctx->pop_render_target();

        needs_redraw = false;
    }

    frame_buffer_style.set_texture(frame_buffer);
    ctx->set_fill_style(&frame_buffer_style);
    ctx->fill_rect({}, (Vec2f) size);
}

void Canvas::add_layer(CanvasLayer* p_layer) {
    layers.push_back(p_layer);
    p_layer->get_layer_update_event_emitter()->add_listener(&handler);
    on_layer_updated();
}

void Canvas::on_layer_updated() {
    needs_redraw = true;
    CanvasUpdateEvent event { this };
    canvas_update_event_emitter.emit(&event);
}