//
// Created by Артем on 15.09.2021.
//

#include "circle_plane_contact.hpp"

namespace Physics {

CirclePlaneContact::CirclePlaneContact(CircleBody* fixture_a, PlaneBody* fixture_b) : Contact(fixture_a,
                                                                                                    fixture_b) {

}

void CirclePlaneContact::response() {
    auto circle = (CircleBody*) get_body_a();
    auto plane = (PlaneBody*) get_body_b();
    auto radius = circle->get_radius();

    auto circle_position = circle->get_position();
    auto circle_velocity = circle->get_velocity();
    auto plane_position = plane->get_position();

    if(plane->get_orientation() == PlaneOrientation::vertical) {
        if(circle_velocity.x > 0 && circle_position.x > plane_position.x - radius &&
           circle_position.x < plane_position.x) {
            circle->add_tick_velocity({-2 * circle_velocity.x, 0});
            return;
        }
        if(circle_velocity.x < 0 && circle_position.x < plane_position.x + radius &&
           circle_position.x > plane_position.x) {
            circle->add_tick_velocity({-2 * circle_velocity.x, 0});
            return;
        }
    } else {
        if(circle_velocity.y > 0 && circle_position.y > plane_position.y - radius &&
           circle_position.y < plane_position.y) {
            circle->add_tick_velocity({0, -2 * circle_velocity.y});
            return;
        }
        if(circle_velocity.y < 0 && circle_position.y < plane_position.y + radius &&
           circle_position.y > plane_position.y) {
            circle->add_tick_velocity({0, -2 * circle_velocity.y});
            return;
        }
    }
}

Contact* CirclePlaneContact::get_contact(CircleBody* fixture_a, PlaneBody* fixture_b) {
    if(test(fixture_a, fixture_b)) {
        return new CirclePlaneContact(fixture_a, fixture_b);
    } else {
        return nullptr;
    }
}

bool CirclePlaneContact::test(CircleBody* circle, PlaneBody* plane) {
    auto radius = circle->get_radius();

    auto circle_position = circle->get_position();
    auto circle_velocity = circle->get_velocity();
    auto plane_position = plane->get_position();

    if(plane->get_orientation() == PlaneOrientation::vertical) {
        if(circle_velocity.x > 0 && circle_position.x > plane_position.x - radius &&
           circle_position.x < plane_position.x)
            return true;
        if(circle_velocity.x < 0 && circle_position.x < plane_position.x + radius &&
           circle_position.x > plane_position.x)
            return true;
    } else {
        if(circle_velocity.y > 0 && circle_position.y > plane_position.y - radius &&
           circle_position.y < plane_position.y)
            return true;
        if(circle_velocity.y < 0 && circle_position.y < plane_position.y + radius &&
           circle_position.y > plane_position.y)
            return true;
    }

    return false;
}

}