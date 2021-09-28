#pragma once

#include "contact.hpp"
#include "../bodies/circle_body.hpp"

namespace Physics {

class CircleCircleContact : public Contact {
public:
    CircleCircleContact(CircleBody* fixture_a, CircleBody* fixture_b);

    static bool test(CircleBody* fixture_a, CircleBody* fixture_b) {
        Vec2d other_pos = fixture_b->get_position();
        other_pos -= fixture_a->get_position();
        double max_distance = fixture_b->radius + fixture_a->radius;
        return other_pos.length_squared() < max_distance * max_distance;
    }

    void response() override;

    static Contact* get_contact(CircleBody* fixture_a, CircleBody* fixture_b);
};

}