//
// Created by Артем on 20.10.2021.
//

#include "canvas_layer.hpp"

void CanvasLayer::set_needs_redraw() {
    needs_redraw = true;
    LayerUpdateEvent event { this };
    layer_update_event_emitter.emit(&event);
}
