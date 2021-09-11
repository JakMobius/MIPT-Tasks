//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_GRAPH_HPP
#define GRAPHICS_ZHIRAPHICS_GRAPH_HPP
#include "drawing_context.hpp"

class Graph;

enum VisualVectorStyle {
    VisualVectorStyleCircled,
    VisualVectorStyleArrowed
};

struct VisualVector {
    Vec2 base;
    Vec2 projections;
    VisualVectorStyle style;

    VisualVector(const Vec2 &base, const Vec2 &projections, VisualVectorStyle style);

    void draw(DrawingContext* ctx) const;
};

typedef float (*plot_function)(float x);

class Graph {
private:
    Matrix3 inverse_coordinate_space;
    Matrix3 coordinate_space;
    Vec2 viewport_center;
    Vec2 viewport_scale;
    int steps;

    Vec2 graph_position;
    Vec2 graph_size;
    float graph_angle;
    plot_function f;
    dynamic_array<VisualVector*> vectors_to_draw;
public:

    Graph(float (*func)(float x), const Vec2& position, const Vec2& size);
    ~Graph();

    void set_viewport(const Vec2 &scale, const Vec2 &center = Vec2(0, 0), float angle = 0);

    void draw(DrawingContext* ctx) const;

    const Matrix3& get_inverse_coordinate_space() const { return inverse_coordinate_space; };
    const Matrix3& get_coordinate_space() const { return coordinate_space; };

    int get_steps() const { return steps; }
    void set_steps(int steps) { this->steps = steps; }

    plot_function get_function() const { return this->f; }
    void set_function(plot_function f) { this->f = f; }

    const Vec2& get_graph_position() const { return this->graph_position; }
    void set_graph_position(Vec2& position) { this->graph_position.set(position); }

    const Vec2& get_graph_size() const { return this->graph_size; }
    void set_graph_size(Vec2& position) { this->graph_size.set(position); }

    float get_graph_angle() const { return this->graph_angle; }
    void set_graph_angle(float angle) { this->graph_angle = angle; }

    void add_vector(VisualVector* vector) { this->vectors_to_draw.push(vector); }
    bool remove_vector(VisualVector* vector) {
        for(int i = 0; i < this->vectors_to_draw.size; i++) {
            if(this->vectors_to_draw.get(i) == vector) {
                this->vectors_to_draw.erase(i);
                return true;
            }
        }
        return false;
    }

private:
    Graph& operator=(const Graph& other);
    Graph(const Graph& other);

    void draw_text(DrawingContext* ctx, float value, const Vec2 &text_position) const;
    void draw_zeroes(DrawingContext* ctx) const;
    void draw_labels(DrawingContext* ctx) const;
    void draw_background(DrawingContext* ctx) const;
    void draw_function(DrawingContext* ctx) const;

    void interface_coordinate_system(DrawingContext* ctx) const;

    void algebraic_coordinate_system(DrawingContext* ctx) const;
};

void draw_vector_c_style(Graph* thou, DrawingContext* ctx, const VisualVector* vector);

#endif //GRAPHICS_ZHIRAPHICS_GRAPH_HPP
