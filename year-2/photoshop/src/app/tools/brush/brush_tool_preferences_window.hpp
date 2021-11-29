#pragma once

#include "../../../utils/vec2.hpp"
#include "../../../ui/window/ui_window.hpp"
#include "brush_tool.hpp"

class BrushToolPreferencesWindow : public UIWindow {

    BrushTool* tool;

public:
    BrushToolPreferencesWindow(BrushTool* tool, const Vec2f& position): UIWindow(position, {100, 100}), tool(tool) {
        set_title("Brush preferences");
    }
};