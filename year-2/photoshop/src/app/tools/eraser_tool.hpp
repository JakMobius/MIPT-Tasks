#pragma once

#include "brush/brush_tool.hpp"

class EraserTool : public BrushTool {

public:
    EraserTool(): BrushTool() {};

    void prepare_to_draw() override;
    void setup_color_blending() override;
};