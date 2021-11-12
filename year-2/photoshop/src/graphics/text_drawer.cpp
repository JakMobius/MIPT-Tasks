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
    sf::Transform transform(t[0], t[3], t[6], t[1], t[4], t[7], t[2], t[5], t[8]);

    position += text_position;
    sf_text.setPosition(position[0], position[1]);
    ctx->get_render_target()->get_target()->draw(sf_text, transform);
}

Vec2f TextDrawer::get_char_position(int char_index) {
    auto position = sf_text.findCharacterPos(char_index);
    return { position.x, position.y };
}

Vec2f TextDrawer::get_text_bounds() {
    auto bounds = sf_text.getLocalBounds();
    if(bounds.height < (float)get_font_size()) bounds.height = (float)get_font_size();
    return {bounds.width, bounds.height};
}
