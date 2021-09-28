#pragma once

namespace Physics {
class World;
};

#include "../../utils/dynamic_array.hpp"
#include "../contacts/contact.hpp"

namespace Physics {
class World {
    dynamic_array<RigidBody*> bodies {};
    dynamic_array<Contact*> contacts {};

private:

    void single_tick(double dt);

public:

    World() = default;

    ~World();

    World(const World &other) = delete;

    World(World &&other) = delete;

    World &operator=(const World* other) = delete;

    World &operator=(World &&other) = delete;

    void tick(double dt, int steps);

    void draw(RigidBody* ctx);

    void add_body(RigidBody* body);

    void delete_body(RigidBody* body);

    const dynamic_array<RigidBody*>& get_bodies() const;

    double get_total_kinetic_energy() const;
};
}