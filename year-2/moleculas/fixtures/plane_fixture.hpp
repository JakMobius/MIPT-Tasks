#pragma once

#include "fixture.hpp"

enum class PlaneOrientation {
    horizontal,
    vertical
};

class PlaneFixture : public Fixture {
    PlaneOrientation orientation;
    Vec2d size;
public:

    PlaneFixture(const Vec2d& center, PlaneOrientation orientation): Fixture(ColliderType::plane, center), orientation(orientation) {}
    Contact* intersects(const Fixture* other);

    PlaneOrientation get_orientation() { return orientation; }
};