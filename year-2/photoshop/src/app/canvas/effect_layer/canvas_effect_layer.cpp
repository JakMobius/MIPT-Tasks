//
// Created by Артем on 11.11.2021.
//

#include "canvas_effect_layer.hpp"
#include "../canvas.hpp"

CatmullRomMapGenerator<float> CanvasEffectLayer::generator { 256, 32 };

void CanvasEffectLayer::create_preferences_generator() {
    preferences_generator = new EffectLayerPreferencesGenerator(this);
}

void CanvasEffectLayer::draw(Canvas* canvas, DrawingContext* ctx) {
    auto old_canvas_framebuffer = canvas->get_framebuffer();
    canvas->set_framebuffer(swap_texture);
    swap_texture = old_canvas_framebuffer;

    ctx->pop_render_target();
    ctx->push_render_target(canvas->get_framebuffer());
    ctx->clear({0, 0, 0, 0});

    shader.setUniform("texture", sf::Shader::CurrentTexture);
    shader.setUniform("back_buffer", *old_canvas_framebuffer->get_texture());

    shader.setUniformArray("red",   &united_map[0 * generator.resolution], generator.resolution);
    shader.setUniformArray("green", &united_map[1 * generator.resolution], generator.resolution);
    shader.setUniformArray("blue",  &united_map[2 * generator.resolution], generator.resolution);

    CanvasLayer::draw(canvas, ctx);
}
