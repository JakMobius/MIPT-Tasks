#pragma once

class ToolSelectWindow;

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/texture.hpp"
#include "../assets.hpp"
#include "../tools/tool_manager.hpp"
#include "../app.hpp"
#include "photoshop_window.hpp"
#include "color_select_button.hpp"

struct ToolManagerColorListener : public EventHandler<ToolManagerColorEvent> {
    ToolSelectWindow* window;
public:
    explicit ToolManagerColorListener(ToolSelectWindow* window): window(window) {}

    void operator() (ToolManagerColorEvent* event) override;
};

struct ToolButtonStyle : public UIButtonStyle {
    UIFillStyleTexture* style;
    explicit ToolButtonStyle(UIFillStyleTexture* style): style(style) {}
    ~ToolButtonStyle() override { delete style; }

    const UIFillStyle* get_hovered_color()  const override { return style; };
    const UIFillStyle* get_idle_color()     const override { return style; };
    const UIFillStyle* get_clicked_color()  const override { return style; };
    const UIFillStyle* get_selected_color() const override { return style; };
    const UIFillStyle* get_disabled_color() const override { return style; };
    const UIFillStyle* get_inactive_color() const override { return style; };
};

class ToolSelectWindow : public PhotoshopWindow {
    UIStackView* stack = nullptr;
    ToolManager* manager = nullptr;
    ToolManagerColorListener color_listener { this };
    ColorSelectButton* color_select_button;

    void emit_color_picker();

    void create_tool_buttons();

public:
    explicit ToolSelectWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position);
    ~ToolSelectWindow() {
        manager->get_color_event_emitter()->remove_listener(&color_listener);
    };

    void layout() override;

    void on_manager_color_updated();
};