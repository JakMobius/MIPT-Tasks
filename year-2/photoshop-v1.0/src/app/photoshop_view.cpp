//
// Created by Артем on 18.10.2021.
//

#include "photoshop_view.hpp"
#include "ui/action_button.hpp"
#include "ui/colorpicker/colorpicker_window.hpp"
#include "tools/brush/brush_tool.hpp"
#include "tools/eraser_tool.hpp"
#include "ui/tool_select_window.hpp"
#include "ui/effect_select_window.hpp"
#include "ui/layer_inspector_window.hpp"
#include "ui/canvas_creation_window.hpp"
#include "ui/canvas_window.hpp"
#include "canvas/effect_layer/effect_layer_preferences_window.hpp"
#include "ui/file_open_window.hpp"

UIFillStyleColor PhotoshopView::app_background {{ 0.2, 0.2, 0.2, 1 }};

PhotoshopView::PhotoshopView(App* app, const Vec2f &position, const Vec2f &size) : UIView(position, size), app(app) {

    append_child(window_container);
    append_child(action_button_view);

    plugin_manager = new PluginManager(this);

    tool_manager = new ToolManager(this);
    tool_manager->add_tool_factory(new ToolFactory<BrushTool>("Brush", Assets.tool_brush_texture));
    tool_manager->add_tool_factory(new ToolFactory<EraserTool>("Eraser", Assets.tool_rubber_texture));
    tool_manager->set_color({1, 0, 0, 1});

    action_button_view->append_child(new ActionButton("New canvas", [this]() {
        present_canvas_creation_modal();
    }));

    action_button_view->append_child(new ActionButton("Open file", [this]() {
        present_file_open_modal();
    }));

    action_button_view->append_child(new ActionButton("Tools", [this]() {
        open_tool_select_window();
    }));

    action_button_view->append_child(new ActionButton("Effects", [this]() {
        open_effect_select_window();
    }));

    action_button_view->append_child(new ActionButton("Layer inspector", [this]() {
        open_layer_inspector();
    }));

    action_button_view->set_fill_style(&UIViewWhiteBackground);

    plugin_manager->load_plugin_dir("./plugins/");
}

void PhotoshopView::create_canvas(const Vec2f& position, const Vec2f& size) {
    auto* canvas_window = new CanvasWindow(this, tool_manager, position, size);
    canvas_window->get_canvas_view()->create_canvas((Vec2i) size);
    auto canvas = canvas_window->get_canvas_view()->get_canvas();

    auto layer = new CanvasLayer((Vec2i) size);
    canvas->add_layer(layer);
    canvas->set_active_layer(layer);

    window_container->add_window(canvas_window);
}

void PhotoshopView::open_layer_inspector() {
    if(!layer_inspector_window) {
        layer_inspector_window = new LayerInspectorWindow(this, tool_manager, {500, 100});
        open_workspace_window((void**) &layer_inspector_window, (UIWindow*) layer_inspector_window);
    }
    window_container->focus_window(layer_inspector_window);
}

void PhotoshopView::open_tool_select_window() {
    if(!tool_select_window) {
        tool_select_window = new ToolSelectWindow(this, tool_manager, {300, 100});
        open_workspace_window((void**) &tool_select_window, (UIWindow*) tool_select_window);
    }
    window_container->focus_window(tool_select_window);
}

void PhotoshopView::open_effect_select_window() {
    if(!effect_select_window) {
        effect_select_window = new EffectSelectWindow(this, {300, 100});
        open_workspace_window((void**) &effect_select_window, (UIWindow*) effect_select_window);
    }
    window_container->focus_window(effect_select_window);
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
    window_container->focus_window(color_picker);
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

    window->layout();
    window->set_position((screen->get_size() * 0.5) -= (window->get_size() * 0.5));

    EventHandler<WindowCloseEvent>* window_destroy_event = new EventHandler<WindowCloseEvent>;
    EventHandler<ViewDestroyEvent>* overlay_destroy_handler = new EventHandler<ViewDestroyEvent>;

    *window_destroy_event = [window_destroy_event, overlay, overlay_destroy_handler](WindowCloseEvent*){
        overlay->get_destroy_event_emitter()->remove_listener(overlay_destroy_handler);
        overlay->get_container()->close_modal(overlay);
        delete window_destroy_event;
    };

    *overlay_destroy_handler = [window, window_destroy_event](ViewDestroyEvent*) {
        window->get_close_event_emitter()->remove_listener(window_destroy_event);
    };

    overlay->get_destroy_event_emitter()->add_listener(overlay_destroy_handler);
    window->get_close_event_emitter()->add_listener(window_destroy_event);

    screen->present_modal(overlay);

    nested_window_container->focus_window(window);
}

void PhotoshopView::present_window(UIWindow* window) {
    window_container->add_window(window);
}

ToolManager* PhotoshopView::get_tool_manager() {
    return tool_manager;
}

void PhotoshopView::focus_window(UIWindow* window) {
    window_container->focus_window(window);
}

PluginManager* PhotoshopView::get_plugin_manager() {
    return plugin_manager;
}

void PhotoshopView::open_workspace_window(void** window_pointer, UIWindow* window) {
    EventHandler<WindowCloseEvent>* handler = new EventHandler<WindowCloseEvent>;
    *handler = [handler, window_pointer, this](WindowCloseEvent*){
        *window_pointer = nullptr;
        delete handler;
    };
    window->get_close_event_emitter()->add_listener(handler);
    window_container->add_window(window);
}
