#pragma once

#include "tool.hpp"
#include "../assets.hpp"

class EraserTool : public Tool {
    DrawingContext ctx {};
    UIFillStyleColor brush_fill_style {{0, 0, 0, 0}};
    UIStrokeStyleColor brush_stroke_style {{0, 0, 0, 0}};
    UIFillStyleTexture texture_copy_style {};
    UIFillStyleTexture brush_apply_style {};
    Vec2f old_position;
    DrawingTargetTexture* map_texture;
    DrawingTargetTexture* buffer_texture;

    void preserve_color(sf::BlendMode& mode);
    void override_color(sf::BlendMode& mode);
    void override_alpha(sf::BlendMode& mode);
    void multiply_alpha(sf::BlendMode &mode);

public:
    EraserTool();

    void on_mouse_down(Vec2f position) override;
    void on_mouse_move(Vec2f position) override;

    void on_become_active() override;
    void on_resign_active() override;

    void draw(Vec2f position);
};