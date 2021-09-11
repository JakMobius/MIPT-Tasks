//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP
#define GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP

#include "matrix3.hpp"
#include "vec2.hpp"
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

    dynamic_array<sf::Vertex> vertex_buffer;
public:
    Matrix3 transform;
    sf::RenderWindow *window;
    sf::Color color;
    sf::Font font;

    HTextAlignment hAlignment;
    VTextAlignment vAlignment;

    explicit DrawingContext(sf::RenderWindow *window);
    ~DrawingContext();

    void draw_line(Vec2 from, Vec2 to) const;
    void draw_rect(const Vec2& position, const Vec2& size) const;
    void draw_circle(const Vec2& center, float radius);
    void draw_text(Vec2 position, const char* text) const;

private:
    DrawingContext& operator=(const DrawingContext& other);
    DrawingContext(const DrawingContext& other);
};

#endif //GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP
