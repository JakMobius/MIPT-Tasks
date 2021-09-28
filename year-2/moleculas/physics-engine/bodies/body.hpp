#pragma once

namespace Physics {
enum class ColliderType {
    invalid,
    circle,
    plane
};

class RigidBody;
class RigidBodyDelegate;
}

#include "../../utils/vec2.hpp"
#include "../contacts/contact.hpp"
#include "../../components/component.hpp"
#include "../world/world.hpp"

namespace Physics {

class RigidBodyDelegate : public virtual ComponentDelegate {
public:
    virtual bool on_collision(Entity* other) { return true; }
};

class RigidBody : public Component<RigidBodyDelegate> {
    World* world;
    ColliderType type;
    Vec2d velocity;
    Vec2d position;
    Vec2d tick_velocity_modification;
    double mass;
protected:
    explicit RigidBody(RigidBodyDelegate* entity, ColliderType type = ColliderType::invalid, const Vec2d &position = {}, double mass = 1) :
        Component(entity), type(type), velocity(), position(position), mass(mass) {};

public:

    ~RigidBody();

    Contact* intersects(const RigidBody* other);

    const Vec2d &get_velocity() const { return velocity; }

    void set_velocity(const Vec2d &new_velocity) { velocity = new_velocity; }

    const Vec2d &get_position() const { return position; }

    void set_position(const Vec2d &new_position) { position = new_position; }

    ColliderType get_type() const { return type; }

    void add_tick_velocity(const Vec2d &velocity);

    void update_velocity();

    double get_mass() const;

    void set_mass(double mass) { this->mass = mass; }

    void set_world(World* world) { this->world = world; }

    World* get_world() const { return this->world; };

    bool emit_collision(RigidBody* other);
};
}