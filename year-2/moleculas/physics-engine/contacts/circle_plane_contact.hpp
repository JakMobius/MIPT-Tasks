#pragma once

#include "contact.hpp"
#include "../bodies/circle_body.hpp"

namespace Physics {

class CirclePlaneContact : public Contact {
public:
    CirclePlaneContact(CircleBody* fixture_a, PlaneBody* fixture_b);

    static bool test(CircleBody* fixture_a, PlaneBody* fixture_b);

    void response() override;

    static Contact* get_contact(CircleBody* fixture_a, PlaneBody* fixture_b);
};

}