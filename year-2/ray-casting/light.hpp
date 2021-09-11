#pragma once

#include "vec3.hpp"

class LightPoint {
public:
    Vec3d position;
    Vec3d color;
    double intensity;
    explicit LightPoint(const Vec3d &position, double intensity = 1.0, const Vec3d &color = {1.0, 1.0, 1.0}):
                    position(position),
                    color(color),
                    intensity(intensity) {}
};