#pragma once

namespace Physics {
class Contact;
}

#include "../bodies/body.hpp"

namespace Physics {
class Contact {
    RigidBody* fixture_a;
    RigidBody* fixture_b;

public:

    Contact(RigidBody* fixture_a, RigidBody* fixture_b) : fixture_a(fixture_a), fixture_b(fixture_b) {}

    virtual ~Contact() {};

    virtual void response() = 0;

    RigidBody* get_body_a() const { return fixture_a; }

    RigidBody* get_body_b() const { return fixture_b; }
};
}