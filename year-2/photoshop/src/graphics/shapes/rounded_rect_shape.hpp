#pragma once

#include "shape.hpp"

class RoundedRectShape : public Shape {
    Vec2f position;
    Vec2f size;
    Vec4f radii;
    bool valid = false;
    std::vector<Vertex> vertices {};

    void shape_circle(Vec2f circle_position, float radius, float angle_from, float angle_to);
    void reshape();

public:
    RoundedRectShape(const Vec2f& position, const Vec2f& size, const Vec4f& radii): position(position), size(size), radii(radii) {}

    void draw(DrawingContext* ctx) override {
        if(!valid) reshape();
        ctx->fill_shape(vertices, PrimitiveType::triangle_fan);
    }

    void resize(const Vec2f& p_size) {
        size = p_size;
        valid = false;
    }
};