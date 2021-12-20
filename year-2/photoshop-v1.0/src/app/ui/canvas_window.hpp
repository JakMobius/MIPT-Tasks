#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/drawing_target_texture.hpp"
#include "canvas_view.hpp"
#include "../app.hpp"
#include "photoshop_window.hpp"

class CanvasWindow : public PhotoshopWindow {

    CanvasView* canvas_view = nullptr;
    EventHandler<WindowCloseEvent> close_handler { [this](WindowCloseEvent*){ deactivate_canvas(); }};

    void deactivate_canvas();

public:
    explicit CanvasWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});;

    void focus() override;

    CanvasView* get_canvas_view() { return canvas_view; }
};