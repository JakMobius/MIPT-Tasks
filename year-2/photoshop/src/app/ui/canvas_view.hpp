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

class CanvasViewUpdateHandler : public EventHandler<CanvasUpdateEvent> {
    CanvasView* view;
public:
    explicit CanvasViewUpdateHandler(CanvasView* view): view(view) {}
    void operator() (CanvasUpdateEvent* event) override;
};

class CanvasView : public UIView {

    ToolManager* tool_manager = nullptr;
    Canvas* canvas = nullptr;
    CanvasViewUpdateHandler canvas_view_update_listener { this };
    UIFillStyleTexture imaged_background { Assets.canvas_background_texture };

public:
    explicit CanvasView(ToolManager* manager, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}):
        UIView(position, size), tool_manager(manager) {}
    ~CanvasView() override {
        canvas->get_event_emitter()->remove_listener(&canvas_view_update_listener);
        delete canvas;
    }

    void layout() override {
        UIView::layout();
        if(!fill_style) set_fill_style(&imaged_background);
    }

    void create_canvas(Vec2i size) {
        canvas = new Canvas(size);
        canvas->get_event_emitter()->add_listener(&canvas_view_update_listener);
    }

    void draw(DrawingContext* ctx) override {
        UIView::draw(ctx);
        canvas->draw(ctx);
    }

    void on_canvas_updated() {
        set_needs_redraw();
    }

    void set_active(bool p_is_active) override {
        UIView::set_active(p_is_active);
        tool_manager->set_active_canvas(canvas);
    }

    void on_mouse_down(MouseDownEvent* event) override {
        tool_manager->on_mouse_down({ event->x, event->y });
    }

    void on_mouse_move(MouseMoveEvent* event) override {
        tool_manager->on_mouse_move({ event->x, event->y });
    }

    void on_mouse_up(MouseUpEvent* event) override {
        tool_manager->on_mouse_up();
    }

    Canvas* get_canvas() { return canvas; }
    ToolManager* get_tool_manager() { return tool_manager; }
};