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

enum class HTextAlignment {
    left, center, right
};

enum class VTextAlignment {
    top, center, bottom
};

class DrawingContext {

    dynamic_array<sf::Vertex> vertex_buffer;
public:
    Matrix3 transform;
    sf::RenderWindow *window;
    sf::Color color {255, 255, 255, 255};
    sf::Font font;

    HTextAlignment hAlignment = HTextAlignment::left;
    VTextAlignment vAlignment = VTextAlignment::bottom;

    explicit DrawingContext(sf::RenderWindow *window);
    ~DrawingContext();
    DrawingContext(DrawingContext&& other) noexcept;
    DrawingContext& operator=(DrawingContext&& other) noexcept;

    void draw_line(Vec2 from, Vec2 to) const;
    void draw_rect(Vec2 position, Vec2 size) const;
    void draw_circle(Vec2 center, float radius);
    void draw_text(Vec2 position, char* text) const;

private:
    DrawingContext& operator=(const DrawingContext& other);
    DrawingContext(const DrawingContext& other);
};

#endif //GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP
