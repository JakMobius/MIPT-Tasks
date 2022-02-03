#pragma once

#include "shape.hpp"

class LineShape: public Shape {

    std::vector<Vec2f> points;
    std::vector<Vertex> vertices;
    float thickness = 1;
    bool valid = false;

    void reshape();

public:
    LineShape(): Shape() {

    }

    void draw(DrawingContext* ctx) override;
    void set_points(const std::vector<Vec2f>& points);
    void set_points(std::vector<Vec2f>&& points);
    void add_point(const Vec2f& point) { points.push_back(point); valid = false; }
    void set_thickness(float p_thickness) { thickness = p_thickness; valid = false; };

    void clear_shape();
};