#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/drawing_target_texture.hpp"
#include "canvas_view.hpp"

class CanvasWindow : public UIWindow {

    CanvasView* canvas_view = nullptr;

public:
    explicit CanvasWindow(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIWindow(position, size, "Canvas") {
        canvas_view = new CanvasView({}, size);
        get_content_view()->append_child(canvas_view);
    };

    CanvasView* get_canvas_view() { return canvas_view; }
};