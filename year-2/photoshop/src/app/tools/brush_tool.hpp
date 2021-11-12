#pragma once

#include "tool.hpp"
#include "../assets.hpp"

class BrushTool : public Tool {
protected:
    DrawingContext ctx {};
    UIFillStyleColor brush_fill_style {{0, 0, 0, 0}};
    UIStrokeStyleColor brush_stroke_style {{0, 0, 0, 0}};
    UIFillStyleTexture texture_copy_style {};
    UIFillStyleTexture brush_apply_style {};
    Vec2f old_position;
    DrawingTargetTexture* map_texture = nullptr;
    DrawingTargetTexture* buffer_texture = nullptr;

    static void preserve_color(sf::BlendMode& mode);
    static void override_color(sf::BlendMode& mode);
    static void blend_color(sf::BlendMode& mode);
    static void override_alpha(sf::BlendMode& mode);
    static void multiply_alpha(sf::BlendMode& mode);
    static void blend_alpha(sf::BlendMode& mode);

public:
    BrushTool();

    void on_mouse_down(Vec2f position) override;
    void on_mouse_move(Vec2f position) override;

    void on_become_active() override;
    void on_resign_active() override;

    void draw(Vec2f position);
    virtual void prepare_to_draw();
    virtual void setup_color_blending();

    void on_layer_change() override;

    void create_textures();

    void delete_textures();
};

#include "tool_manager.hpp"