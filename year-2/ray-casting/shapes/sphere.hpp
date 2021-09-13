#pragma once

#include "../utils/vec3.hpp"

class Sphere {
public:
    Vec3d center;
    double radius;
    int mirroring;
    Vec3d color = {1, 1, 1};

    Sphere(const Vec3d& center, double radius): center(center), radius(radius), mirroring(5) {}
};