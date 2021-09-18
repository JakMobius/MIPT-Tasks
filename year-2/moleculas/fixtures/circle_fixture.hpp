#pragma once

#include "fixture.hpp"
#include "../utils/vec2.hpp"
#include "circle_fixture.hpp"
#include "plane_fixture.hpp"

struct CircleFixture : public Fixture {
    double radius;

public:
    CircleFixture(const Vec2d& center, double radius, double mass): Fixture(ColliderType::circle, center, mass), radius(radius) {}

    Contact* intersects(const Fixture* other);

    double get_radius() { return radius; }
};