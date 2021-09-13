
#include "ui_text.hpp"

void UIText::draw(DrawingContext* ctx) {
    ctx->vAlignment = v_text_alignment;
    ctx->hAlignment = h_text_alignment;
    ctx->color = text_color.to_sf_color();

    if(str) {
        ctx->draw_text(size * 0.5, str);
    }
}
