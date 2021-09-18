//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP
#define GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP

#include "utils/matrix3.hpp"
#include "utils/vec2.hpp"
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
    Matrix3d transform;
    sf::RenderWindow *window;
    sf::Color color;
    sf::Font font;

    HTextAlignment hAlignment;
    VTextAlignment vAlignment;

    explicit DrawingContext(sf::RenderWindow *window);
    ~DrawingContext();

    void draw_line(Vec2d from, Vec2d to) const;
    void draw_rect(const Vec2d& position, const Vec2d& size) const;
    void draw_circle(const Vec2d& center, float radius);
    void draw_text(Vec2d position, const char* text) const;

    double get_height() { return window->getSize().y; }
    double get_width() { return window->getSize().x; }

    void set_color(double r, double g, double b, double a);

private:
    DrawingContext& operator=(const DrawingContext& other);
    DrawingContext(const DrawingContext& other);
};

#endif //GRAPHICS_ZHIRAPHICS_DRAWING_CONTEXT_HPP
