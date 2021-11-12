//
// Created by Артем on 18.10.2021.
//

#include "photoshop_view.hpp"
#include "ui/action_button.hpp"
#include "ui/colorpicker/colorpicker_window.hpp"
#include "tools/brush_tool.hpp"
#include "tools/eraser_tool.hpp"
#include "ui/tool_select_window.hpp"
#include "ui/layer_inspector_window.hpp"
#include "ui/canvas_creation_window.hpp"
#include "ui/canvas_window.hpp"
#include "canvas/effect_layer/effect_layer_preferences_window.hpp"
#include "ui/file_open_window.hpp"

UIFillStyleColor PhotoshopView::app_background {{ 0.2, 0.2, 0.2, 1 }};

PhotoshopView::PhotoshopView(App* app, const Vec2f &position, const Vec2f &size) : UIView(position, size), app(app) {

    append_child(window_container);
    append_child(action_button_view);

    manager = new ToolManager();
    manager->add_tool_factory(new ToolFactory<BrushTool>("Brush", Assets.tool_brush_texture));
    manager->add_tool_factory(new ToolFactory<EraserTool>("Eraser", Assets.tool_rubber_texture));
    manager->set_color({1, 0, 0, 1});

    action_button_view->append_child(new ActionButton("New canvas", [this]() {
        present_canvas_creation_modal();
    }));

    action_button_view->append_child(new ActionButton("Open file", [this]() {
        present_file_open_modal();
    }));

    action_button_view->append_child(new ActionButton("Tools window", [this]() {
        open_tool_select_window();
    }));

    action_button_view->append_child(new ActionButton("Layer inspector", [this]() {
        open_layer_inspector();
    }));

    action_button_view->set_fill_style(&UIViewWhiteBackground);
}

void PhotoshopView::create_canvas(const Vec2f& position, const Vec2f& size) {
    auto* canvas_window = new CanvasWindow(this, manager, position, size);
    canvas_window->get_canvas_view()->create_canvas((Vec2i) size);
    auto canvas = canvas_window->get_canvas_view()->get_canvas();

    auto layer = new CanvasLayer((Vec2i) size);
    canvas->add_layer(layer);
    canvas->set_active_layer(layer);

    window_container->add_window(canvas_window);
}

void PhotoshopView::open_layer_inspector() {
    if(!layer_inspector_window) {
        layer_inspector_window = new LayerInspectorWindow(this, manager, {500, 100});
        EventHandler<WindowCloseEvent>* handler = new EventHandler<WindowCloseEvent>;
        *handler = [handler, this](WindowCloseEvent*){
            layer_inspector_window = nullptr;
            delete handler;
        };
        layer_inspector_window->get_close_event_emitter()->add_listener(handler);
        window_container->add_window(layer_inspector_window);
    }
    window_container->activate_window(layer_inspector_window);
}

void PhotoshopView::open_tool_select_window() {
    if(!tool_select_window) {
        tool_select_window = new ToolSelectWindow(this, manager, {300, 100});
        EventHandler<WindowCloseEvent>* handler = new EventHandler<WindowCloseEvent>;
        *handler = [handler, this](WindowCloseEvent*){
            tool_select_window = nullptr;
            delete handler;
        };
        tool_select_window->get_close_event_emitter()->add_listener(handler);
        window_container->add_window(tool_select_window);
    }
    window_container->activate_window(tool_select_window);
}

void PhotoshopView::open_colorpicker(const std::function<void(const Vec4f &)> &callback, const Vec4f* current_color = nullptr) {
    if(!color_picker) {
        color_picker = new ColorPickerWindow(this, get_size() * 0.5);
        EventHandler<WindowCloseEvent>* handler = new EventHandler<WindowCloseEvent>;
        *handler = [handler, this](WindowCloseEvent*){
            color_picker = nullptr;
            delete handler;
        };
        color_picker->get_close_event_emitter()->add_listener(handler);
        window_container->add_window(color_picker);
    }
    window_container->activate_window(color_picker);
    if(current_color) color_picker->set_color(*current_color);
    color_picker->set_callback(callback);
}

void PhotoshopView::layout() {
    action_button_view->set_fitting({size[0], {}});
    action_button_view->layout_if_needed();
    window_container->set_position({0, action_button_view->get_size()[1]});
    window_container->set_size(size - Vec2f {0, 25});
    if(!fill_style) set_fill_style(&app_background);
    UIView::layout();
}

void PhotoshopView::present_canvas_creation_modal() {
    auto canvas_create_window = new CanvasCreationWindow(this);
    canvas_create_window->set_callback([this](long width, long height) {
        create_canvas({100, 100},{ (float)width, (float)height });
    });

    present_window_modally(canvas_create_window);
}

void PhotoshopView::present_file_open_modal() {
    auto canvas_create_window = new FileOpenWindow(this);

    present_window_modally(canvas_create_window);
}

void PhotoshopView::present_window_modally(UIWindow* window) {
    auto screen = get_screen();
    auto nested_window_container = new UIWindowContainer();
    auto overlay = new UIModalViewOverlay();

    overlay->set_stretched_child(nested_window_container);

    nested_window_container->add_window(window);
    nested_window_container->activate_window(window);

    window->layout();
    window->set_position((screen->get_size() * 0.5) -= (window->get_size() * 0.5));

    EventHandler<WindowCloseEvent>* handler = new EventHandler<WindowCloseEvent>;
    *handler = [handler, overlay](WindowCloseEvent*){
        overlay->get_container()->close_modal(overlay);
        delete handler;
    };

    window->get_close_event_emitter()->add_listener(handler);

    screen->present_modal(overlay);
}

void PhotoshopView::present_window(UIWindow* window) {
    window_container->add_window(window);
}

ToolManager* PhotoshopView::get_tool_manager() {
    return manager;
}

void PhotoshopView::focus_window(UIWindow* window) {
    window_container->activate_window(window);
}
