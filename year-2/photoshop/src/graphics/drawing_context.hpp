#pragma once

#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "drawing_target.hpp"
#include "drawing_target_texture.hpp"
#include "../utils/vec4.hpp"
#include "../ui/styles/fill_style.hpp"
#include "../ui/styles/stroke_style.hpp"
#include "text_drawer.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

struct Vertex {
    Vec2f position;
    Vec2f shape_position;
};

enum class PrimitiveType {
    points =         sf::PrimitiveType::Points,
    lines =          sf::PrimitiveType::Lines,
    line_strip =     sf::PrimitiveType::LineStrip,
    triangles =      sf::PrimitiveType::Triangles,
    triangle_strip = sf::PrimitiveType::TriangleStrip,
    triangle_fan =   sf::PrimitiveType::TriangleFan,
    quads =          sf::PrimitiveType::Quads
};

class DrawingContext {

    std::vector<sf::Vertex> vertex_buffer {32};

    const UIFillStyle* fill_style = nullptr;
    const UIStrokeStyle* stroke_style = nullptr;

public:

    explicit DrawingContext();
    ~DrawingContext();

    Matrix3f transform {};
    std::vector<DrawingTarget*> target_stack {};
    DrawingTarget* target = nullptr;

    void stroke_line(Vec2f from, Vec2f to, float thickness = 1) const;
    void fill_rect(const Vec2f& position, const Vec2f& size) const;
    void fill_circle(const Vec2f& center, float radius);
    void fill_shape(const std::vector<Vertex>& shape, PrimitiveType type = PrimitiveType::triangles);
    void fill_shape(const std::vector<sf::Vertex>& shape, PrimitiveType type = PrimitiveType::triangles);

    void clear(const Vec4f &color);

    void push_render_target(DrawingTarget* target);
    void pop_render_target();

    void set_stroke_style(const UIStrokeStyle* p_stroke_style) { stroke_style = p_stroke_style; }
    void set_fill_style(const UIFillStyle* p_fill_style) { fill_style = p_fill_style; }

    DrawingTarget* get_render_target();

private:
    DrawingContext& operator=(const DrawingContext& other);
};
