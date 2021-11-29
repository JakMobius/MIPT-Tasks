#pragma once

#include "../../preferences_generator.hpp"
#include "brush_tool.hpp"
#include "brush_tool_preferences_window.hpp"

class BrushToolPreferencesGenerator;



class BrushToolPreferencesGenerator : public PreferencesGenerator {
    BrushTool* tool;
    BrushToolPreferencesWindow* opened_window = nullptr;
public:
    explicit BrushToolPreferencesGenerator(BrushTool* tool): PreferencesGenerator(), tool(tool) {}
    ~BrushToolPreferencesGenerator() override;

    void open_preferences(PhotoshopView* app) override;
};