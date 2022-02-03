//
// Created by Артем on 29.11.2021.
//

#include "circle-shape.hpp"

void CircleShape::reshape() {
    const int steps = 30;

    const float angle_step = M_PI * 2 / (float) steps;

    for(int i = 0; i < steps; i++) {
        float angle = angle_step * (float) i;
        Vec2f shape_position = {(float) sin(angle) * radius, (float) cos(angle) * radius};
        Vec2f screen_position(shape_position);
        screen_position += position;
        vertices.push_back({screen_position, shape_position});
    }
}
