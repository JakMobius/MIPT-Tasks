#pragma once

#include "contact.hpp"
#include "../fixtures/circle_fixture.hpp"

class CircleCircleContact : public Contact {
public:
    CircleCircleContact(CircleFixture* fixture_a, CircleFixture* fixture_b);

    static bool test(CircleFixture* fixture_a, CircleFixture* fixture_b) {
        Vec2d other_pos = fixture_b->get_position();
        other_pos -= fixture_a->get_position();
        double max_distance = fixture_b->radius + fixture_a->radius;
        return other_pos.length_squared() < max_distance * max_distance;
    }

    void response() override;

    static Contact* get_contact(CircleFixture* fixture_a, CircleFixture* fixture_b);
};