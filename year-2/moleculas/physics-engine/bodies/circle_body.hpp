#pragma once

#include "body.hpp"
#include "../../utils/vec2.hpp"
#include "circle_body.hpp"
#include "plane_body.hpp"

namespace Physics {
struct CircleBody : public RigidBody {
    double radius;

public:
    CircleBody(RigidBodyDelegate* entity, const Vec2d& center, double radius, double mass): RigidBody(entity, ColliderType::circle, center, mass), radius(radius) {}

    Contact* intersects(const RigidBody* other);

    double get_radius() { return radius; }

    void set_radius(double radius) { this->radius = radius; }
};
}