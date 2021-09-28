//
// Created by Артем on 15.09.2021.
//

#include "circle_circle_contact.hpp"

namespace Physics {

CircleCircleContact::CircleCircleContact(CircleBody* fixture_a, CircleBody* fixture_b) : Contact(fixture_a,
                                                                                                 fixture_b) {

}

void CircleCircleContact::response() {
    auto circle_a = (CircleBody*) get_body_a();
    auto circle_b = (CircleBody*) get_body_b();

    Vec2d radius_unit = circle_b->get_position();
    radius_unit -= circle_a->get_position();
    radius_unit /= radius_unit.length();
    Vec2d tangential_unit {-radius_unit.y, radius_unit.x};

    Vec2d a_velocity_transformed = circle_a->get_velocity();
    Vec2d b_velocity_transformed = circle_b->get_velocity();

    Vec2d collision_point_velocity = circle_a->get_velocity() * circle_a->get_mass();
    collision_point_velocity += circle_b->get_velocity() * circle_b->get_mass();
    collision_point_velocity /= (circle_a->get_mass() + circle_b->get_mass());

    a_velocity_transformed -= collision_point_velocity;
    b_velocity_transformed -= collision_point_velocity;

    a_velocity_transformed = {
            a_velocity_transformed.dot(tangential_unit) * circle_a->get_mass(),
            a_velocity_transformed.dot(radius_unit) * circle_a->get_mass()
    };

    b_velocity_transformed = {
            b_velocity_transformed.dot(tangential_unit) * circle_b->get_mass(),
            b_velocity_transformed.dot(radius_unit) * circle_b->get_mass()
    };

    if((a_velocity_transformed.y < 0) || (b_velocity_transformed.y > 0)) return;

    double tmp = a_velocity_transformed.y;
    a_velocity_transformed.y = b_velocity_transformed.y;
    b_velocity_transformed.y = tmp;

    Vec2d new_a_velocity = (tangential_unit * a_velocity_transformed.x) += (radius_unit * a_velocity_transformed.y);
    Vec2d new_b_velocity = (tangential_unit * b_velocity_transformed.x) += (radius_unit * b_velocity_transformed.y);

    new_a_velocity /= circle_a->get_mass();
    new_b_velocity /= circle_b->get_mass();

    new_a_velocity += collision_point_velocity;
    new_b_velocity += collision_point_velocity;

    new_a_velocity -= circle_a->get_velocity();
    new_b_velocity -= circle_b->get_velocity();

    circle_a->add_tick_velocity(new_a_velocity);
    circle_b->add_tick_velocity(new_b_velocity);
}

Contact* CircleCircleContact::get_contact(CircleBody* fixture_a, CircleBody* fixture_b) {
    if(test(fixture_a, fixture_b)) {
        return new CircleCircleContact(fixture_a, fixture_b);
    } else {
        return nullptr;
    }
}

}