#pragma once

class LayerInspectorWindow;

#include "../canvas/canvas_layer.hpp"
#include "../photoshop_view.hpp"
#include "../../ui/ui_scroll_view.hpp"
#include "../canvas/canvas.hpp"
#include "../tools/tool_manager.hpp"
#include "photoshop_window.hpp"

class LayerListItemView : public UIView {
    static UIFillStyleColor selected_background;
    LayerInspectorWindow* window;
    CanvasLayer* previewed_layer = nullptr;
    UIView* layer_present_view = nullptr;
    UIView* layer_present_view_background = nullptr;
    UIView* layer_present_view_container = nullptr;
    UIFillStyleTexture* layer_preview_view_style = new UIFillStyleTexture();
    UIFillStyleTexture imaged_background { Assets.canvas_background_texture };

    bool is_layer_active = false;

    EventHandler<LayerUpdateEvent> layer_update_event_listener {[this](LayerUpdateEvent*) {
        set_needs_redraw();
    }};

public:
    LayerListItemView(LayerInspectorWindow* window);
    ~LayerListItemView() override;

    void preview_layer(CanvasLayer* layer);
    void update_texture();
    void update_background();
    void on_mouse_click(MouseClickEvent* event) override;
};

class LayerInspectorWindow : public PhotoshopWindow {
    Canvas* active_canvas = nullptr;
    UIScrollView* layers_scroll_view = nullptr;
    ToolManager* manager = nullptr;
    UIStackView* bottom_button_container = nullptr;
    UIButton* effect_layer_add_button = nullptr;
    UIButton* layer_add_button = nullptr;
    UIButton* layer_remove_button = nullptr;
    UIButton* layer_preferences_button = nullptr;
    std::vector<LayerListItemView*> layer_list_item_views {};

    EventHandler<CanvasChangeEvent> canvas_change_event_listener {[this](CanvasChangeEvent*) {
        on_canvas_changed();
    }};

    EventHandler<CanvasUpdateEvent> canvas_update_event_listener {[this](CanvasUpdateEvent*) {
        on_canvas_changed();
    }};

    void on_canvas_changed();
    void update_list();
    void update_list_size(int n);

public:
    LayerInspectorWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position);
    ~LayerInspectorWindow();

    ToolManager* get_manager() { return manager; }

    void setup_bottom_bar();
};
