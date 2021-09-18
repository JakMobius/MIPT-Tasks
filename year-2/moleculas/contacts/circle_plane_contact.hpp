#pragma once

#include "contact.hpp"
#include "../fixtures/circle_fixture.hpp"
#include "../fixtures/plane_fixture.hpp"
#include "../fixtures/circle_fixture.hpp"

class CirclePlaneContact : public Contact {
public:
    CirclePlaneContact(CircleFixture* fixture_a, PlaneFixture* fixture_b);

    static bool test(CircleFixture* fixture_a, PlaneFixture* fixture_b);

    void response() override;

    static Contact* get_contact(CircleFixture* fixture_a, PlaneFixture* fixture_b);
};