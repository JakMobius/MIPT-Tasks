#pragma once

#include "plugin_std.hpp"
#include "../../graphics/drawing_target.hpp"
#include "../../graphics/drawing_target_texture.hpp"
#include "plugin.hpp"
#include "../canvas/canvas_layer.hpp"

class HostRenderTarget : public PUPPY::RenderTarget {

    DrawingTargetTexture* target;
    Plugin* plugin;
    CanvasLayer* layer = nullptr;
    UIFillStyleTexture shared_fill_style {};

public:
    HostRenderTarget(DrawingTargetTexture* target, Plugin* plugin): target(target), plugin(plugin) {}

    RenderTarget* get_copy() const override;

    PUPPY::Vec2s get_size() const override;

    PUPPY::RGBA get_pixel(size_t x, size_t y) const override;
    void set_pixel(size_t x, size_t y, const PUPPY::RGBA &color) override;

    PUPPY::RGBA* get_pixels() const override;

    void set_texture(DrawingTargetTexture* p_texture);

    void clear(const PUPPY::RGBA &color) override;

    void render_circle(const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) override;
    void render_line(const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) override;
    void render_triangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::Vec2f &p3, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) override;
    void render_rectangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) override;
    void render_texture(const PUPPY::Vec2f &position, const RenderTarget* texture, const PUPPY::RenderMode &render_mode) override;
    void render_pixels(const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA* data, const PUPPY::RenderMode &render_mode) override;

    void setup_render_mode(const PUPPY::RenderMode& mode);

    void fuse_context(const PUPPY::RGBA &color, const PUPPY::RenderMode& mode);

    void unfuse_context();

    void apply_shader(const PUPPY::Shader* shader) override;
    void link_to_layer(CanvasLayer* p_layer) { layer = p_layer; };

    void fill_shape(Vec2f* shape, int count, PrimitiveType type);

    DrawingTargetTexture* get_texture() const { return target; }
};