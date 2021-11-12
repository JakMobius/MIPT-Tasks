//
// Created by Артем on 19.10.2021.
//

#include "canvas_window.hpp"

void CanvasWindow::deactivate_canvas() {
    if(canvas_view->get_tool_manager()->get_active_canvas() == this->canvas_view->get_canvas()) {
        canvas_view->get_tool_manager()->set_active_canvas(nullptr);
    }
}

CanvasWindow::CanvasWindow(PhotoshopView* app, ToolManager* manager, const Vec2f &position, const Vec2f &size) : PhotoshopWindow(app, position, size, "Canvas") {
    canvas_view = new CanvasView(manager, {}, size);

    get_content_view()->append_child(canvas_view);
    get_close_event_emitter()->add_listener(&close_handler);
}

void CanvasWindow::focus() {
    PhotoshopWindow::focus();
    canvas_view->focus();
}
