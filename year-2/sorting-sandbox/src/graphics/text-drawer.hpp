#pragma once

class DrawingContext;
class TextDrawer;

#include <SFML/Graphics/Text.hpp>
#include "../app/assets.hpp"
#include "drawing-context.hpp"

enum TextAlignment {
    TextAlignmentLeading,
    TextAlignmentCenter,
    TextAlignmentTrailing
};

enum HTextAlignment {
    HTextAlignmentLeft   = TextAlignmentLeading,
    HTextAlignmentCenter = TextAlignmentCenter,
    HTextAlignmentRight  = TextAlignmentTrailing
};

enum VTextAlignment {
    VTextAlignmentTop    = TextAlignmentLeading,
    VTextAlignmentCenter = TextAlignmentCenter,
    VTextAlignmentBottom = TextAlignmentTrailing
};

class TextDrawer {
    sf::Text sf_text;
    const char* text = nullptr;
    HTextAlignment h_alignment = HTextAlignmentLeft;
    VTextAlignment v_alignment = VTextAlignmentTop;

    Vec2f size;

    Vec2f iterate_position(Vec2f position, char previous_char, char current_char);

public:
    explicit TextDrawer(const Vec2f& size = {}, const char* text = ""):
        size(size),
        sf_text(text, *Assets.default_font, 15) {}

    Vec2f get_text_bounds();

    void set_font_size(int p_size) { sf_text.setCharacterSize(p_size); }
    int get_font_size() { return sf_text.getCharacterSize(); }

    void set_size(const Vec2f& p_size) { size = p_size; }
    Vec2f get_size() const { return size; }

    void set_text(const char* p_text) { text = p_text; sf_text.setString(text); }

    void set_h_alignment(HTextAlignment alignment) { h_alignment = alignment; }
    void set_v_alignment(VTextAlignment alignment) { v_alignment = alignment; }
    void set_font_color(const Vec4f& color) { sf_text.setFillColor(color.to_sf_color()); }

    HTextAlignment get_h_alignment() const { return h_alignment; }
    VTextAlignment get_v_alignment() const { return v_alignment; }

    void draw(DrawingContext* ctx, Vec2f position);

    Vec2f get_char_position(int char_index);
    int get_char_index(Vec2f position);

    const char* get_text();
};