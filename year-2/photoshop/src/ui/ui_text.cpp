
#include "ui_text.hpp"
#include "../graphics/drawing_context.hpp"

void UIText::draw(DrawingContext* ctx) {
    ctx->vAlignment = v_text_alignment;
    ctx->hAlignment = h_text_alignment;
    ctx->set_text_color(text_color);

    if(str) {
        ctx->stroke_text(size * 0.5, str);
    }
}
