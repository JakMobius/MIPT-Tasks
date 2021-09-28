#pragma once

#include "body.hpp"

namespace Physics {
enum class PlaneOrientation {
    horizontal,
    vertical
};

class PlaneBody : public RigidBody {
    PlaneOrientation orientation;
    Vec2d size;
public:

    PlaneBody(RigidBodyDelegate* entity, const Vec2d &center, PlaneOrientation orientation) :
        RigidBody(entity, ColliderType::plane, center),
        orientation(orientation) {}

    Contact* intersects(const RigidBody* other);

    PlaneOrientation get_orientation() { return orientation; }
};
}