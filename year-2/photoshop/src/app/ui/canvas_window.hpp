#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/drawing_target_texture.hpp"
#include "canvas_view.hpp"
#include "../app.hpp"
#include "photoshop_window.hpp"

class CanvasWindow : public PhotoshopWindow {

    CanvasView* canvas_view = nullptr;

public:
    explicit CanvasWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): PhotoshopWindow(app, position, size, "Canvas") {
        canvas_view = new CanvasView(manager, {}, size);
        get_content_view()->append_child(canvas_view);
    };

    CanvasView* get_canvas_view() { return canvas_view; }
};