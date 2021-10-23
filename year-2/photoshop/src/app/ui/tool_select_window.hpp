#pragma once

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/drawable_texture.hpp"
#include "../assets.hpp"
#include "../tools/tool_manager.hpp"
#include "../app.hpp"
#include "photoshop_window.hpp"

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

    void emit_color_picker() {
        auto container = get_container_view();
        if(!container) return;

        get_app()->open_colorpicker([this](const Vec4f& color) {
            manager->set_color(color);
        });
    }

    void create_tool_buttons();

public:
    explicit ToolSelectWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position);
    ~ToolSelectWindow() {};

    void layout() override;
};