#pragma once

class Fixture;

#include "../utils/vec2.hpp"
#include "../contacts/contact.hpp"

enum class ColliderType {
    circle, plane, invalid
};

class Fixture {
    ColliderType type;
    Vec2d velocity;
    Vec2d position;
    Vec2d tick_velocity_modification;
    double mass;
protected:
    explicit Fixture(ColliderType type = ColliderType::invalid, const Vec2d& position = {}, double mass = 1): type(type), velocity(), position(position), mass(mass) {};

public:

    Contact* intersects(const Fixture* other);
    const Vec2d& get_velocity() const { return velocity; }
    void set_velocity(const Vec2d& new_velocity) { velocity = new_velocity; }
    const Vec2d& get_position() const { return position; }
    void set_position(const Vec2d& new_position) { position = new_position; }
    ColliderType get_type() const { return type; }

    Contact* handle_intersection(Fixture* other_fixture);
    void add_tick_velocity(const Vec2d& velocity);

    void update_velocity();

    double get_mass() const;
};