#include "body.hpp"
#include "circle_body.hpp"

namespace Physics {
Contact* RigidBody::intersects(const RigidBody *other) {
    switch(type) {
        case ColliderType::circle:
            assert(&CircleBody::intersects != &RigidBody::intersects);
            return ((CircleBody*) this)->intersects(other);
        case ColliderType::plane:
            assert(&PlaneBody::intersects != &RigidBody::intersects);
            return ((PlaneBody*) this)->intersects(other);
        default: assert(false);
    }
}

void RigidBody::add_tick_velocity(const Vec2d &velocity) {
    tick_velocity_modification += velocity;
}

void RigidBody::update_velocity() {
    velocity += tick_velocity_modification;
    tick_velocity_modification *= 0;
}

double RigidBody::get_mass() const {
    return mass;
}

RigidBody::~RigidBody() {
    if(world) {
        world->delete_body(this);
    }
}

bool RigidBody::emit_collision(RigidBody* other) {
    bool collide_a = other->get_delegate()->on_collision(this->get_entity());
    bool collide_b = this->get_delegate()->on_collision(other->get_entity());

    return collide_a && collide_b;
}
}