#pragma once

#include "shape.hpp"

class CircleShape : public Shape {

    Vec2f position;
    float radius;
    bool valid = false;
    std::vector<Vertex> vertices {};

    void reshape();

public:
    CircleShape(const Vec2f& position, float radius): position(position), radius(radius) {}

    void draw(DrawingContext* ctx) override {
        if(!valid) reshape();
        ctx->fill_shape(vertices, PrimitiveType::triangle_fan);
    }

    void set_position(const Vec2f& p_position) {
        position = p_position;
        valid = false;
    }

    void set_radius(float p_radius) {
        p_radius = radius;
        valid = false;
    }
};