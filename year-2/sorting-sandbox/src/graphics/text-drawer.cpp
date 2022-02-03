//
// Created by Артем on 29.10.2021.
//

#include "text-drawer.hpp"

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

int TextDrawer::get_char_index(Vec2f click_position) {
    if(!text) return 0;
    Vec2f previous_position = {};
    Vec2f char_position = {};

    auto position = sf_text.getInverseTransform().transformPoint(click_position[0], click_position[1]);

    char previous = '\0';
    float character_size = (float)sf_text.getCharacterSize();

    int i = 0;

    for(; text[i]; i++) {
        char_position = iterate_position(char_position, previous, text[i]);
        previous = text[i];

        if(i > 0) {
            if(char_position[1] == previous_position[1]) {
                float y = char_position[1];
                if(position.y >= y && position.y < y + character_size &&
                    position.x > previous_position[0] && position.x <= char_position[0]) {
                    if(position.x < previous_position[0] / 2 + char_position[0] / 2) return i;
                    return i + 1;
                }
            } else {
                float y = previous_position[1];
                if(position.y >= y && position.y < y + character_size && position.x > previous_position[0]) return i;
            }
        }

        previous_position = char_position;
    }

    if(position.y >= previous_position[1] && position.y < previous_position[1] + character_size &&
       position.x > previous_position[0]) {
        return i;
    }

    return 0;
}

Vec2f TextDrawer::iterate_position(Vec2f position, char previous_char, char current_char) {
    if(current_char == '\r') return position;
    float x = position[0], y = position[1];
    auto font = sf_text.getFont();
    float line_spacing = font->getLineSpacing(sf_text.getCharacterSize()) * sf_text.getLineSpacing();
    x += font->getKerning(previous_char, current_char, sf_text.getCharacterSize());
    if(current_char == '\t') {
        x += font->getGlyph(L' ', sf_text.getCharacterSize(), false).advance * 4;
    } else if(current_char == '\n') {
        x = 0;
        y += (float)sf_text.getCharacterSize() * line_spacing;
    } else {
        x += font->getGlyph(current_char, sf_text.getCharacterSize(), false).advance + sf_text.getLetterSpacing();
    }
    return {x, y};
}

const char* TextDrawer::get_text() {
    return text;
}
