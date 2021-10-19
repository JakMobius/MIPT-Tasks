#pragma once

#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
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

    std::vector<sf::Vertex> vertex_buffer;
public:
    Matrix3f transform;
    sf::RenderWindow *window;
    sf::Color color;
    sf::Font font;

    HTextAlignment hAlignment;
    VTextAlignment vAlignment;

    explicit DrawingContext(sf::RenderWindow *window);
    ~DrawingContext();

    void draw_line(Vec2f from, Vec2f to, float thickness = 1) const;
    void draw_rect(const Vec2f& position, const Vec2f& size) const;
    void draw_circle(const Vec2f& center, float radius);
    void draw_text(Vec2f position, const char* text) const;

private:
    DrawingContext& operator=(const DrawingContext& other);
    DrawingContext(sf::Window* other);
};
