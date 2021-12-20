//
// Created by Артем on 11.11.2021.
//

#include "line_shape.hpp"

void LineShape::reshape() {
    vertices.clear();

    for(int i = 1; i < points.size(); i++) {
        Vec2f& from = points[i - 1];
        Vec2f& to = points[i];

        Vec2f ortho_offset = { to[1] - from[1], - to[0] + from[0] };
        ortho_offset.normalize();
        ortho_offset *= thickness / 2;

        Vec2f shape[] = { from, from, to, to };

        shape[0] -= ortho_offset;
        shape[1] += ortho_offset;
        shape[2] += ortho_offset;
        shape[3] -= ortho_offset;

        float position = float(i);

        vertices.push_back({shape[0], {position - 1, 1}});
        vertices.push_back({shape[1], {position - 1, -1}});
        vertices.push_back({shape[3], {position, 1}});

        vertices.push_back({shape[1], {position - 1, -1}});
        vertices.push_back({shape[2], {position, -1}});
        vertices.push_back({shape[3], {position, 1}});
    }

    valid = true;
}

void LineShape::draw(DrawingContext* ctx) {
    if(!valid) reshape();
    ctx->fill_shape(vertices, PrimitiveType::triangles);
}

void LineShape::set_points(const std::vector<Vec2f>& p_points) {
    points = p_points;
    valid = false;
}

void LineShape::set_points(std::vector<Vec2f>&& p_points) {
    points = p_points;
    valid = false;
}

void LineShape::clear_shape() {
    points.clear();
    valid = false;
}
