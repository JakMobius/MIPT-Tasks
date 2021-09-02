//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_GRAPH_HPP
#define GRAPHICS_ZHIRAPHICS_GRAPH_HPP

#include "drawing_context.hpp"

enum class VisualVectorStyle {
    circled, arrowed
};

struct VisualVector {
    Vec2 base;
    Vec2 projections;
    VisualVectorStyle style;

    VisualVector(const Vec2 &base, const Vec2 &projections, VisualVectorStyle style);
};

class Graph {
private:
    Matrix3 inverse_coordinate_space;
    Matrix3 coordinate_space;
    Vec2 viewport_center;
    Vec2 viewport_scale;
public:

    Vec2 graph_position;
    Vec2 graph_size;
    float graph_angle;
    float (*f)(float x) = nullptr;
    dynamic_array<VisualVector> vectors_to_draw;

    Graph(float (*func)(float x), Vec2 position, Vec2 size);
    ~Graph();
    Graph(Graph&& other) noexcept;
    Graph& operator=(Graph&& other) noexcept;

    void set_viewport(const Vec2 &scale, const Vec2 &center = {0, 0}, float angle = 0);

    void draw(DrawingContext* ctx) const;

private:
    Graph& operator=(const Graph& other);
    Graph(const Graph& other);

    void draw_text(DrawingContext* ctx, float value, const Vec2 &text_position) const;
    void draw_zeroes(DrawingContext* ctx) const;
    void draw_labels(DrawingContext* ctx) const;
    void draw_background(DrawingContext* ctx) const;
    void draw_function(DrawingContext* ctx) const;

    void transform_matrix(DrawingContext* ctx) const;

    void draw_vector(DrawingContext* ctx, const VisualVector &vector) const;
};

#endif //GRAPHICS_ZHIRAPHICS_GRAPH_HPP
