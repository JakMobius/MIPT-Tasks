#pragma once

#include "../ui/ui_view.hpp"
#include "../ui/window/ui_window.hpp"
#include "../ui/window/ui_window_container.hpp"
#include "ui/canvas_window.hpp"
#include "tools/brush_tool.hpp"
#include "ui/tool_select_window.hpp"
#include "tools/eraser_tool.hpp"

class PhotoshopView : public UIWindowContainer {
public:
    explicit PhotoshopView(const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0}): UIWindowContainer(position, size) {

        auto* win1 = new CanvasWindow({100, 100}, {400, 400});
        auto* win2 = new ToolSelectWindow({500, 100});

        win1->get_canvas_view()->create_canvas({400, 400});

        auto canvas = win1->get_canvas_view()->get_canvas();

        auto layer = new CanvasLayer({400, 400});
        canvas->add_layer(layer);
        canvas->set_active_layer(layer);

        add_window(win1);
        add_window(win2);

        ToolManager* manager = win1->get_canvas_view()->get_tool_manager();

        manager->add_tool_factory(new ToolFactory<BrushTool>("Brush", Assets.tool_brush_texture));
        manager->add_tool_factory(new ToolFactory<EraserTool>("Eraser", Assets.tool_rubber_texture));

        win2->set_tool_manager(manager);

        manager->set_color({1, 0, 0, 1});

    }
};