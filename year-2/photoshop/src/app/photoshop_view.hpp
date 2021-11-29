#pragma once

class PhotoshopView;
class LayerInspectorWindow;
class ToolSelectWindow;
class ToolManager;
class CanvasWindow;

#include "../utils/vec4.hpp"
#include "../ui/ui_view.hpp"
#include "../ui/window/ui_window.hpp"
#include "../ui/window/ui_window_container.hpp"
#include "plugin/plugin_manager.hpp"
#include <functional>

class ColorPickerWindow;
class App;

class PhotoshopView : public UIView {

    static UIFillStyleColor app_background;

    ColorPickerWindow* color_picker = nullptr;
    UIStackView* action_button_view = new UIStackView(UIAxis::x);
    UIWindowContainer* window_container = new UIWindowContainer();
    App* app = nullptr;
    ToolManager* tool_manager = nullptr;
    ToolSelectWindow* tool_select_window = nullptr;
    LayerInspectorWindow* layer_inspector_window = nullptr;
    PluginManager* plugin_manager = nullptr;

    void create_canvas(const Vec2f &position, const Vec2f &size);
    void present_canvas_creation_modal();
    void present_file_open_modal();

    void open_tool_select_window();
    void open_layer_inspector();

public:
    explicit PhotoshopView(App* app, const Vec2f& position = {0, 0}, const Vec2f& size = {0, 0});

    void layout() override;

    void present_window_modally(UIWindow* window);
    void present_window(UIWindow* window);
    void focus_window(UIWindow* window);
    void open_colorpicker(const std::function<void(const Vec4f &)> &callback, const Vec4f* current_color);

    ToolManager* get_tool_manager();
    PluginManager* get_plugin_manager();
};