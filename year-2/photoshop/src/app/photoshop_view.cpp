//
// Created by Артем on 18.10.2021.
//

#include "photoshop_view.hpp"

#include "ui/canvas_window.hpp"
#include "tools/brush_tool.hpp"
#include "ui/tool_select_window.hpp"
#include "tools/eraser_tool.hpp"
#include "ui/colorpicker/colorpicker_window.hpp"

PhotoshopView::PhotoshopView(App* app, const Vec2f &position, const Vec2f &size) : UIWindowContainer(position, size), app(app) {

    manager = new ToolManager();
    manager->add_tool_factory(new ToolFactory<BrushTool>("Brush", Assets.tool_brush_texture));
    manager->add_tool_factory(new ToolFactory<EraserTool>("Eraser", Assets.tool_rubber_texture));
    manager->set_color({1, 0, 0, 1});

    create_canvas({100, 100}, {400, 400});
    create_canvas({100, 500}, {300, 300});

    add_window(new ToolSelectWindow(this, manager, {510, 100}));
}

void PhotoshopView::create_canvas(const Vec2f& position, const Vec2f& size) {
    auto* win1 = new CanvasWindow(this, manager, position, size);
    win1->get_canvas_view()->create_canvas((Vec2i) size);
    auto canvas = win1->get_canvas_view()->get_canvas();

    auto layer = new CanvasLayer((Vec2i) size);
    canvas->add_layer(layer);
    canvas->set_active_layer(layer);

    add_window(win1);
}

void PhotoshopView::open_colorpicker(const std::function<void(const Vec4f &)> &callback, const Vec4f* current_color = nullptr) {
    if(!color_piсker) create_color_picker();
    activate_window(color_piсker);
    if(current_color) color_piсker->set_color(*current_color);
    color_piсker->set_callback(callback);
}

void PhotoshopView::create_color_picker() {
    color_piсker = new ColorPickerWindow(this, get_size() * 0.5);
    color_piсker->get_close_event_emitter()->add_listener(&color_picker_close_listener);
    add_window(color_piсker);
}

void PhotoshopView::on_color_picker_closed() {
    color_piсker = nullptr;
}

void ColorPickerCloseListener::operator()(WindowCloseEvent* event) {
    view->on_color_picker_closed();
}