#pragma once

class CanvasView;

#include "../../graphics/drawing_context.hpp"
#include "../../graphics/drawing_target_texture.hpp"
#include "../../utils/vec2.hpp"
#include "../../ui/ui_view.hpp"
#include "../canvas/canvas.hpp"
#include "../canvas/canvas_layer.hpp"
#include "../tools/tool_manager.hpp"
#include "../assets.hpp"

class CanvasView : public UIView {
    UIFillStyleTexture imaged_background { Assets.canvas_background_texture };
    ToolManager* tool_manager = nullptr;
    Canvas* canvas = nullptr;
    EventHandler<CanvasUpdateEvent> canvas_view_update_listener {[this](CanvasUpdateEvent*){ on_canvas_updated(); }};

public:
    explicit CanvasView(ToolManager* manager, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}):
        UIView(position, size), tool_manager(manager) {}
    ~CanvasView() override;

    void layout() override;

    void create_canvas(Vec2i size);

    void draw(DrawingContext* ctx) override;

    void on_canvas_updated();

    void focus() override;

    void on_mouse_down(MouseDownEvent* event) override;
    void on_mouse_move(MouseMoveEvent* event) override;
    void on_mouse_up(MouseUpEvent* event) override;

    Canvas* get_canvas() { return canvas; }
    ToolManager* get_tool_manager() { return tool_manager; }
};