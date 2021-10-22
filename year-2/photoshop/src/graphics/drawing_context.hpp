#pragma once

#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "drawing_target.hpp"
#include "drawing_target_texture.hpp"
#include "../utils/vec4.hpp"
#include "../ui/styles/fill_style.hpp"
#include "../ui/styles/stroke_style.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

enum HTextAlignment {
    HTextAlignmentLeft,
    HTextAlignmentCenter,
    HTextAlignmentRight
};

enum VTextAlignment {
    VTextAlignmentTop,
    VTextAlignmentCenter,
    VTextAlignmentBottom
};

class DrawingContext {

    std::vector<sf::Vertex> vertex_buffer {32};

    const UIFillStyle* fill_style = nullptr;
    const UIStrokeStyle* stroke_style = nullptr;
    sf::Color text_color {255, 255, 255, 255};

public:

    explicit DrawingContext();
    ~DrawingContext();

    Matrix3f transform {};
    std::vector<DrawingTarget*> target_stack {};
    DrawingTarget* target = nullptr;
    sf::Font* font;

    HTextAlignment hAlignment;
    VTextAlignment vAlignment;

    void stroke_line(Vec2f from, Vec2f to, float thickness = 1) const;
    void fill_rect(const Vec2f& position, const Vec2f& size) const;
    void fill_circle(const Vec2f& center, float radius);
    void stroke_text(Vec2f position, const char* text) const;
    void draw_texture(Vec2f position, Vec2f size, Drawable* texture);
    void clear(const Vec4f &color);

    void push_render_target(DrawingTarget* target);
    void pop_render_target();

    void set_text_color(const Vec4f& p_text_color) { text_color = p_text_color.to_sf_color(); }
    void set_stroke_style(const UIStrokeStyle* p_stroke_style) { stroke_style = p_stroke_style; }
    void set_fill_style(const UIFillStyle* p_fill_style) { fill_style = p_fill_style; }
    void set_font(sf::Font* p_font) { font = p_font; }

    DrawingTarget* get_render_target();

private:
    DrawingContext& operator=(const DrawingContext& other);
};
