//
// Created by Артем on 29.10.2021.
//

#include "text_drawer.hpp"

void TextDrawer::draw(DrawingContext* ctx, Vec2f position) {
    Vec2f text_position {};

    auto text_bounds = get_text_bounds();
    auto box_bounds = get_size();
    TextAlignment alignment[] = { (TextAlignment) get_h_alignment(), (TextAlignment) get_v_alignment() };

    for(int i = 0; i < 2; i++) {
        switch(alignment[i]) {
            case TextAlignmentCenter:   text_position.set(i, box_bounds[i] / 2 - text_bounds[i] / 2); break;
            case TextAlignmentTrailing: text_position.set(i, box_bounds[i] - text_bounds[i]); break;
            default: break;
        }
    }

    float* t = ctx->transform.data;
    sf::Transform transform(t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8]);
    // Dirty, but is there any other way to do it?
    memcpy((void*) sf_text.getTransform().getMatrix(), transform.getMatrix(),16 * sizeof(float));

    position += text_position;
    sf_text.setPosition(position[0], position[1]);
    ctx->get_render_target()->get_target()->draw(sf_text);
}
