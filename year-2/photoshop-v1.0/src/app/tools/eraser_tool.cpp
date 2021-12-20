//
// Created by Артем on 21.10.2021.
//

#include "eraser_tool.hpp"

void EraserTool::setup_color_blending() {
    auto& brush_fill_mode = brush_fill_style.get_render_states()->blendMode;
    override_color(brush_fill_mode);
    override_alpha(brush_fill_mode);

    auto& brush_stroke_mode = brush_stroke_style.get_render_states()->blendMode;
    override_color(brush_stroke_mode);
    override_alpha(brush_stroke_mode);

    auto& texture_copy_mode = texture_copy_style.get_render_states()->blendMode;
    override_color(texture_copy_mode);
    override_alpha(texture_copy_mode);

    auto& apply_mode = brush_apply_style.get_render_states()->blendMode;
    preserve_color(apply_mode);
    multiply_alpha(apply_mode);
}

void EraserTool::prepare_to_draw() {
    map_texture->clear({1, 1, 1, 1});
    brush_fill_style.set_color({0, 0, 0, 0});
    brush_stroke_style.set_color({0, 0, 0, 0});
}
