//
// Created by Артем on 15.09.2021.
//

#include "world.hpp"

void World::draw(DrawingContext* ctx) {
    for(int i = 0; i < creatures.size; i++) {
        creatures[i]->draw(ctx);
    }
}

void World::tick(double dt, int steps) {
    dt /= (double) steps;
    for(int i = 0; i < steps; i++) {
        single_tick(dt);
    }
}

void World::insert_creature(Creature* circle) {
    creatures.push(circle);
}

void World::single_tick(double dt) {
    contacts.clear();

    for(int i = 0; i < creatures.size; i++) {
        auto creature = creatures[i];
        auto fixture = creature->get_fixture();
        Vec2d velocity = fixture->get_velocity();
        velocity *= dt;
        fixture->set_position(fixture->get_position() + velocity);
    }

    for(int i = 0; i < creatures.size; i++) {
        auto creature_a = creatures[i];
        auto fixture_a = creature_a->get_fixture();
        for(int j = i + 1; j < creatures.size; j++) {
            auto creature_b = creatures[j];
            auto fixture_b = creature_b->get_fixture();

            Contact* contact = fixture_a->intersects(fixture_b);
            if(contact) contacts.push(contact);
        }
    }

    for(int i = 0; i < contacts.size; i++) {
        contacts[i]->response();
        delete contacts[i];
    }

    for(int i = 0; i < creatures.size; i++) {
        auto creature = creatures[i];
        auto fixture = creature->get_fixture();

        fixture->update_velocity();
    }
}

World::~World() {
    for(int i = 0; i < creatures.size; i++) {
        delete creatures[i];
    }
}

const dynamic_array<Creature*> World::get_creatures() const {
    return creatures;
}

double World::get_total_kynetic_energy() const {
    double energy = 0;

    for(int i = 0; i < get_creatures().size; i++) {
        Creature* creature = get_creatures()[i];
        energy += creature->get_fixture()->get_velocity().length_squared() * creature->get_fixture()->get_mass();
    }

    return energy;
}
