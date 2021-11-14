//
// Created by Артем on 19.10.2021.
//

#include "canvas_view.hpp"

CanvasView::~CanvasView() {
    canvas->get_update_event_emitter()->remove_listener(&canvas_view_update_listener);
    delete canvas;
}

void CanvasView::layout() {
    UIView::layout();
    if(!fill_style) set_fill_style(&imaged_background);
}

void CanvasView::create_canvas(Vec2i size) {
    canvas = new Canvas(size);
    canvas->get_update_event_emitter()->add_listener(&canvas_view_update_listener);
}

void CanvasView::draw(DrawingContext* ctx) {
    UIView::draw(ctx);
    canvas->draw(ctx);
}

void CanvasView::on_canvas_updated() {
    set_needs_redraw();
}

void CanvasView::focus() {
    UIView::focus();
    tool_manager->set_active_canvas(canvas);
}

void CanvasView::on_mouse_down(MouseDownEvent* event) {
    tool_manager->on_mouse_down({ event->x, event->y });
}

void CanvasView::on_mouse_move(MouseMoveEvent* event) {
    tool_manager->on_mouse_move({ event->x, event->y });
}

void CanvasView::on_mouse_up(MouseUpEvent* event) {
    tool_manager->on_mouse_up();
}
