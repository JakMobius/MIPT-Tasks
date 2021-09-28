//
// Created by Артем on 15.09.2021.
//

#include "world.hpp"

namespace Physics {
void World::tick(double dt, int steps) {
    dt /= (double) steps;
    for(int i = 0; i < steps; i++) {
        single_tick(dt);
    }
}

void World::add_body(RigidBody* body) {
    bodies.push(body);
    body->set_world(this);
}

void World::single_tick(double dt) {
    contacts.clear();

    for(int i = 0; i < bodies.size; i++) {
        auto body = bodies[i];
        Vec2d velocity = body->get_velocity();
        velocity *= dt;
        body->set_position(body->get_position() + velocity);
    }

    for(int i = 0; i < bodies.size; i++) {
        auto body_a = bodies[i];
        for(int j = i + 1; j < bodies.size; j++) {
            auto body_b = bodies[j];

            Contact* contact = body_a->intersects(body_b);
            if(contact) contacts.push(contact);
        }
    }

    for(int i = 0; i < contacts.size; i++) {
        auto body_a = contacts[i]->get_body_a();
        auto body_b = contacts[i]->get_body_b();

        if(body_a->emit_collision(body_b)) contacts[i]->response();

        delete contacts[i];
    }

    for(int i = 0; i < bodies.size; i++) {
        auto body = bodies[i];

        body->update_velocity();
    }
}

World::~World() {
    for(int i = 0; i < bodies.size; i++) {
        delete bodies[i];
    }
}

const dynamic_array<RigidBody*>& World::get_bodies() const {
    return bodies;
}

double World::get_total_kinetic_energy() const {
    double energy = 0;

    for(int i = 0; i < get_bodies().size; i++) {
        RigidBody* body = get_bodies()[i];
        energy += body->get_velocity().length_squared() * body->get_mass();
    }

    return energy;
}

void World::delete_body(RigidBody* body) {
    bool found = false;

    for(int i = 0; i < bodies.size; i++) {
        if(bodies[i] != body) continue;
        bodies.erase(i);
        found = true;
        break;
    }

    if(!found) return;

    body->set_world(nullptr);
}
}