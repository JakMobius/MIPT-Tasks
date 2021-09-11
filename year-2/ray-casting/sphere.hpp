#pragma once

#include "vec3.hpp"
#include "ray.hpp"

class Sphere {
public:
    Vec3d center;
    double radius;
    int mirroring;
    Vec3f color = {1, 1, 1};

    Sphere(const Vec3d& center, double radius): center(center), radius(radius), mirroring(5) {}
};