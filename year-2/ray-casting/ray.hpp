#pragma once

#include "utils/vec3.hpp"

class Ray {
public:
    Vec3d position;
    Vec3d velocity;
    Vec3d color = {0.01, 0.01, 0.1};

    Ray(const Vec3d &position, const Vec3d &velocity): position(position), velocity(velocity) {};
};