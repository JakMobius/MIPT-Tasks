//
// Created by Артем on 24.10.2021.
//

#include "rounded_rect_shape.hpp"

void RoundedRectShape::shape_circle(Vec2f circle_position, float radius, float angle_from, float angle_to) {
    const int steps = 30;

    const float angle_step = (angle_to - angle_from) / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i + angle_from;
        Vec2f vertex_position = { (float) cos(angle) * radius, (float) sin(angle) * radius };
        vertex_position += circle_position;
        Vec2f shape_position { vertex_position };
        vertex_position += position;
        shape_position.set_y(size[1] - shape_position[1]);

        vertices.push_back({vertex_position, shape_position});
    }
}

void RoundedRectShape::reshape() {
    vertices.clear();
    shape_circle({size[0] - radii[0], radii[0]}, radii[0], -M_PI / 2, 0);
    shape_circle({size[0] - radii[1], size[1] - radii[1]}, radii[1], 0, M_PI / 2);
    shape_circle({radii[2], size[1] - radii[2]}, radii[2], M_PI / 2, M_PI);
    shape_circle({radii[3], radii[3]}, radii[3], M_PI, M_PI * 3 / 2);
    valid = true;
}
