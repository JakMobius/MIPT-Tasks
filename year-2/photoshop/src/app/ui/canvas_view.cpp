//
// Created by Артем on 19.10.2021.
//

#include "canvas_view.hpp"

void CanvasViewUpdateHandler::operator()(CanvasUpdateEvent* event) {
    view->on_canvas_updated();
}
