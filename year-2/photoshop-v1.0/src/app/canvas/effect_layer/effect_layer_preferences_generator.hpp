#pragma once

class EffectLayerPreferencesGenerator;

#include "../../preferences_generator.hpp"
#include "canvas_effect_layer.hpp"
#include "effect_layer_preferences_window.hpp"

class EffectLayerPreferencesGenerator : public PreferencesGenerator {
    CanvasEffectLayer* layer;
    EffectLayerPreferencesWindow* opened_window = nullptr;
public:
    explicit EffectLayerPreferencesGenerator(CanvasEffectLayer* layer): PreferencesGenerator(), layer(layer) {}
    ~EffectLayerPreferencesGenerator() override;

    void open_preferences(PhotoshopView* app) override;
};