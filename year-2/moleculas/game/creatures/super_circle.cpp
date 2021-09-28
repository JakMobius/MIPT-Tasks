//
// Created by Артем on 25.09.2021.
//

#include "super_circle.hpp"

void SuperCircle::eat_ball(Circle* ball) {
    eaten_energy += ball->get_physical_body()->get_velocity().length_squared() * ball->get_physical_body()->get_mass() / 2;
    eaten_balls++;
    auto body = (Physics::CircleBody*)get_physical_body();
    body->set_radius(sqrt(eaten_balls) * 10);
    if(eaten_balls == 1) body->set_mass(ball->get_physical_body()->get_mass());
    else body->set_mass(ball->get_physical_body()->get_mass() + body->get_mass());
}

bool SuperCircle::on_collision(Entity* other) {
    if(!scene->are_reactions_on()) return true;
    if(is_dead()) return false;

    if(other->get_type() == EntityType::super_ball) {
        return explode_with_other_circle((SuperCircle*) other);
    } else if(other->get_type() == EntityType::small_ball) {
        return eat_other_circle((Circle*) other);
    }

    return true;
}

bool SuperCircle::eat_other_circle(Circle* other) {
    if(other->is_dead()) return false;

    auto* a_physical = (Physics::CircleBody*) get_physical_body();
    auto* b_physical = (Physics::CircleBody*) other->get_physical_body();

    Vec2d velocity_a = a_physical->get_velocity();
    Vec2d velocity_b = b_physical->get_velocity();

    double mass_a = a_physical->get_mass();
    double mass_b = b_physical->get_mass();

    velocity_a *= mass_a;
    velocity_b *= mass_b;

    velocity_a += velocity_b;
    velocity_a /= (mass_a + mass_b);
    velocity_a -= a_physical->get_velocity();

    a_physical->add_tick_velocity(velocity_a);
    other->kill();
    eat_ball(other);

    return true;
}

bool SuperCircle::explode_with_other_circle(SuperCircle* other) {
    if(other->is_dead()) return false;

    auto* other_circle = (SuperCircle*) other;
    auto* a_physical = (Physics::CircleBody*) get_physical_body();
    auto* b_physical = (Physics::CircleBody*) other_circle->get_physical_body();

    double total_energy = get_eaten_energy() + other_circle->get_eaten_energy();

    Vec2d position_a = a_physical->get_position();
    Vec2d position_b = b_physical->get_position();

    Vec2d collision_point = position_a * a_physical->get_radius();
    collision_point += (position_b * b_physical->get_radius());
    collision_point /= (a_physical->get_radius() + b_physical->get_radius());

    int total_eaten_balls = get_eaten_balls() + other_circle->get_eaten_balls();

    kill();
    other_circle->kill();

    double each_offset = 30;
    double each_mass = 1;
    double each_energy = total_energy / total_eaten_balls;
    double each_speed = sqrt(each_energy / each_mass * 2);

    for(int i = 0; i < total_eaten_balls; i++) {
        double angle = i * M_PI * 2 / total_eaten_balls;

        Vec2d offset(cos(angle), sin(angle));

        auto* new_circle = new Circle(collision_point + offset * each_offset, 10, each_mass);
        new_circle->get_physical_body()->set_velocity(offset * each_speed);
        scene->add_creature(new_circle);
    }

    return false;
}
