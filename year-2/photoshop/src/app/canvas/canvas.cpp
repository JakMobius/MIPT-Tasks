//
// Created by Артем on 20.10.2021.
//

#include "canvas.hpp"

void Canvas::remove_layer(CanvasLayer* p_layer) {
    p_layer->get_layer_update_event_emitter()->remove_listener(&handler);
    for(int i = 0; i < layers.size(); i++) {
        if(layers[i] == p_layer) {
            layers.erase(layers.begin() + i);
            break;
        }
    }
}

void Canvas::draw(DrawingContext* ctx) {
    for(int i = 0; i < layers.size(); i++) {
        layers[i]->draw(ctx);
    }
}

void Canvas::add_layer(CanvasLayer* p_layer) {
    layers.push_back(p_layer);
    p_layer->get_layer_update_event_emitter()->add_listener(&handler);
}

void Canvas::on_layer_updated() {
    CanvasUpdateEvent event { this };
    canvas_update_event_emitter.emit(&event);
}

void LayerUpdateEventHandler::operator()(LayerUpdateEvent* event) {
    canvas->on_layer_updated();
}
