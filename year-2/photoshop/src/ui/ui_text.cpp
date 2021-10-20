
#include "ui_text.hpp"

void UIText::draw(UIDrawingContext* ctx) {
    ctx->vAlignment = v_text_alignment;
    ctx->hAlignment = h_text_alignment;
    ctx->set_color(text_color);

    if(str) {
        ctx->draw_text(size * 0.5, str);
    }
}
