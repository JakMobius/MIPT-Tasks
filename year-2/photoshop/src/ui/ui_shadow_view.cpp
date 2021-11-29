//
// Created by Артем on 25.10.2021.
//

#include "ui_shadow_view.hpp"

void UIShadowView::draw(DrawingContext* ctx) {
    UIFillStyleTexture fill_style {};
    Vec2f size = parent->get_size();
    ctx->set_fill_style(&fill_style);
    Vec2f position = {10, 10};
    size -= position;

    float inset = 18;
    float shadow_size = 57;

    fill_style.set_color({1, 1, 1, 0.5});
    fill_style.set_texture(Assets.window_shadow_texture);
    ctx->fill_rect(position + Vec2f {inset, -shadow_size + inset}, {size[0] - inset * 2, shadow_size});

    fill_style.set_scale({1, -1});
    ctx->fill_rect(position + Vec2f {inset, size[1] - inset + shadow_size}, {size[0] - inset * 2, -shadow_size});

    fill_style.set_scale({1, 1});
    fill_style.set_texture(Assets.window_shadow_horizontal_texture);
    ctx->fill_rect(position + Vec2f {size[0] - inset, inset}, {shadow_size, size[1] - inset * 2});

    fill_style.set_scale({-1, 1});
    ctx->fill_rect(position + Vec2f {+inset, inset}, {-shadow_size, size[1] - inset * 2});

    fill_style.set_texture(Assets.window_shadow_corner_texture);
    fill_style.set_scale({1, 1});
    ctx->fill_rect(position + Vec2f {size[0] - inset, -shadow_size + inset}, {shadow_size, shadow_size});

    fill_style.set_scale({-1, 1});
    ctx->fill_rect(position + Vec2f {inset, -shadow_size + inset}, {-shadow_size, shadow_size});

    fill_style.set_scale({1, -1});
    ctx->fill_rect(position + Vec2f {size[0] - inset, size[1] - inset + shadow_size}, {shadow_size, -shadow_size});

    fill_style.set_scale({-1, -1});
    ctx->fill_rect(position + Vec2f {inset, size[1] - inset + shadow_size}, {-shadow_size, -shadow_size});

//        ctx->fill_rect({-80 + inset, -80 + inset}, {40, 40});
}
