#pragma once

class CanvasEffectLayer;

#include "../../../utils/vec2.hpp"
#include "../canvas_layer.hpp"
#include "effect_layer_preferences_generator.hpp"
#include "catmull_rom_curve.hpp"
#include "catmull_map_generator.hpp"

class CanvasEffectLayer : public CanvasLayer {

    static CatmullRomMapGenerator<float> generator;
    float* united_map = nullptr;
    CatmullRomCurve curves[3] {};
    sf::Shader shader;
    DrawingTargetTexture* swap_texture = nullptr;

    void create_preferences_generator() override;

    void regenerate_curve(int channel) {
        generator.generate(&united_map[channel * generator.resolution], curves[channel]);
    }

public:
    explicit CanvasEffectLayer(Vec2i size);
    ~CanvasEffectLayer() override;

    void clear_texture() override;

    void draw(Canvas* canvas, DrawingContext *ctx) override;

    const CatmullRomCurve& get_curve(int channel) { return curves[channel]; }
    void set_curve(int channel, const CatmullRomCurve& p_curve) {
        curves[channel] = p_curve;
        regenerate_curve(channel);
        set_needs_redraw();
    }
};