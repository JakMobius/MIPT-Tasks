#pragma once

#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "drawing_target.hpp"
#include "drawing_target_texture.hpp"
#include "../utils/vec4.hpp"
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
    sf::Color color {};

public:

    explicit DrawingContext();
    ~DrawingContext();

    Matrix3f transform {};
    std::vector<DrawingTarget*> target_stack {};
    DrawingTarget* target = nullptr;
    sf::Font font {};

    HTextAlignment hAlignment;
    VTextAlignment vAlignment;

    void draw_line(Vec2f from, Vec2f to, float thickness = 1) const;
    void draw_rect(const Vec2f& position, const Vec2f& size) const;
    void draw_circle(const Vec2f& center, float radius);
    void draw_text(Vec2f position, const char* text) const;
    void draw_texture(Vec2f position, Vec2f size, DrawingTargetTexture* texture);
    void clear();

    void push_render_target(DrawingTarget* target);
    void pop_render_target();

    void set_color(const Vec4f& p_color) { color = p_color.to_sf_color(); }

    DrawingTarget* get_render_target();

private:
    DrawingContext& operator=(const DrawingContext& other);

};
