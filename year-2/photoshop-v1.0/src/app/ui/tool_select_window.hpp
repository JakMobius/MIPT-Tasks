#pragma once

class ToolSelectWindow;

#include "../../ui/window/ui_window.hpp"
#include "../../graphics/texture.hpp"
#include "../assets.hpp"
#include "../tools/tool_manager.hpp"
#include "../app.hpp"
#include "photoshop_window.hpp"
#include "color_select_button.hpp"

class ToolSelectWindow : public PhotoshopWindow {
    UIStackView* stack = nullptr;
    ToolManager* manager = nullptr;
    ColorSelectButton* color_select_button;

    EventHandler<ToolManagerColorEvent> color_listener;

    void emit_color_picker();
    void create_tool_buttons();

public:
    explicit ToolSelectWindow(PhotoshopView* app, ToolManager* manager, const Vec2f& position);
    ~ToolSelectWindow();;

    void layout() override;
};