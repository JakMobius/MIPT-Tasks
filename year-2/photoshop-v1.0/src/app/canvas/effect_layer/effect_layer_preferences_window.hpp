#pragma once

class EffectLayerPreferencesWindow;
class CatmullRomColorCurveView;

#include "../../../ui/window/ui_window.hpp"
#include "../../../graphics/shapes/line_shape.hpp"
#include "catmull_rom_curve.hpp"
#include "canvas_effect_layer.hpp"
#include "catmull_rom_view.hpp"


class EffectLayerPreferencesWindow: public UIWindow {
    CanvasEffectLayer* layer;
    CatmullRomColorCurveView* catmull_rom_views[3] = {};
    UIButton* catmull_rom_buttons[3] = {};

public:
    EffectLayerPreferencesWindow(CanvasEffectLayer* layer);


    void handle_button_click(int i);
};