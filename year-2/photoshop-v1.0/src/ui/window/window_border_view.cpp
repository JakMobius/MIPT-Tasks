//
// Created by Артем on 28.10.2021.
//

#include "window_border_view.hpp"
#include "../styles/fill_style/fill_style_texture.hpp"

void WindowBorderView::draw(DrawingContext* ctx) {
    UIFillStyleTexture fill_style {};
    Vec2f size = parent->get_size();
    ctx->set_fill_style(&fill_style);

    float corner_radius = 18;
    float border_width = 1;

    fill_style.set_color({1, 1, 1, 0.3});
    fill_style.set_texture(Assets.window_border_vertical_texture);
    ctx->fill_rect(Vec2f {0, corner_radius}, {border_width, size[1] - corner_radius * 2});

    fill_style.set_scale({-1, 1});
    ctx->fill_rect(Vec2f {size[0], corner_radius}, {-border_width, size[1] - corner_radius * 2});

    fill_style.set_scale({1, 1});
    fill_style.set_texture(Assets.window_border_horizontal_texture);
    ctx->fill_rect(Vec2f { corner_radius, 0 }, {size[0] - corner_radius * 2, border_width });

    fill_style.set_scale({1, -1});
    ctx->fill_rect(Vec2f { corner_radius, size[1] }, {size[0] - corner_radius * 2, -border_width });

//        fill_style.set_color({1, 1, 1, 1});
    fill_style.set_texture(Assets.window_border_corner_texture);
    fill_style.set_scale({1, 1});
    ctx->fill_rect(Vec2f {size[0] - corner_radius, 0}, {corner_radius, corner_radius});

    fill_style.set_scale({-1, 1});
    ctx->fill_rect(Vec2f {corner_radius, 0}, {-corner_radius, corner_radius});

    fill_style.set_scale({1, -1});
    ctx->fill_rect(Vec2f {size[0] - corner_radius, size[1]}, {corner_radius, -corner_radius});

    fill_style.set_scale({-1, -1});
    ctx->fill_rect(Vec2f {corner_radius, size[1]}, {-corner_radius, -corner_radius});
}
