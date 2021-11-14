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

CanvasEffectLayer::CanvasEffectLayer(Vec2i size) : CanvasLayer(size) {
    united_map = new float[generator.resolution * 3];
    swap_texture = new DrawingTargetTexture(size);
    shader.loadFromFile("resources/shaders/rgb_mapping_effect_shader.shader", sf::Shader::Fragment);
    draw_style.get_render_states()->shader = &shader;
    draw_style.get_render_states()->blendMode = sf::BlendNone;

    for(int i = 0; i < 3; i++) regenerate_curve(i);
}

CanvasEffectLayer::~CanvasEffectLayer() {
    delete united_map;
    delete swap_texture;
}

void CanvasEffectLayer::clear_texture() {
    texture->clear({1, 1, 1, 1});
    needs_clear = false;
}
