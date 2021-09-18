#pragma once

#include "utils/dynamic_array.hpp"
#include "creatures/creature.hpp"
#include "creatures/circle.hpp"
#include "contacts/contact.hpp"
#include "fixtures/fixture.hpp"

class World {
    dynamic_array<Creature*> creatures {};
    dynamic_array<Contact*> contacts {};

    void single_tick(double dt);
public:

    World() = default;
    ~World();
    World(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(const World* other) = delete;
    World& operator=(World&& other) = delete;

    void tick(double dt, int steps);
    void draw(DrawingContext* ctx);

    void insert_creature(Creature* circle);

    void handle_intersection(Fixture* fixture_a, Fixture* fixture_b);

    const dynamic_array<Creature*> get_creatures() const;

    double get_total_kynetic_energy() const;
};