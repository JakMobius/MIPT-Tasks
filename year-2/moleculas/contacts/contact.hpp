#pragma once

class Contact;

#include "../fixtures/fixture.hpp"

class Contact {
    Fixture* fixture_a;
    Fixture* fixture_b;

public:

    Contact(Fixture* fixture_a, Fixture* fixture_b): fixture_a(fixture_a), fixture_b(fixture_b) {}
    virtual ~Contact() {};

    virtual void response() = 0;

    Fixture* get_fixture_a() const { return fixture_a; }
    Fixture* get_fixture_b() const { return fixture_b; }
};