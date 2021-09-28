//
// Created by Артем on 15.09.2021.
//

#include "circle.hpp"

bool Circle::on_collision(Entity* other) {
    if(!scene->are_reactions_on()) return true;
    if(is_dead()) return false;

    if(other->get_type() == EntityType::small_ball) {
        auto* other_circle = (Circle*) other;
        if(other_circle->is_dead()) return false;

        auto* a_physical = (Physics::CircleBody*) get_physical_body();
        auto* b_physical = (Physics::CircleBody*) other_circle->get_physical_body();

        Vec2d velocity_a = a_physical->get_velocity();
        Vec2d velocity_b = b_physical->get_velocity();

        double mass_a = a_physical->get_mass();
        double mass_b = b_physical->get_mass();

        velocity_a *= mass_a;
        velocity_b *= mass_b;

        Vec2d position_a = a_physical->get_position();
        Vec2d position_b = b_physical->get_position();

        Vec2d collision_point = position_a * a_physical->get_radius();
        collision_point += (position_b * b_physical->get_radius());
        collision_point /= (a_physical->get_radius() + b_physical->get_radius());

        auto* new_circle = new SuperCircle(collision_point);

        new_circle->eat_ball(this);
        new_circle->eat_ball(other_circle);

        Vec2d total_velocity = velocity_a + velocity_b;
        total_velocity /= new_circle->get_physical_body()->get_mass();

        scene->add_creature(new_circle);
        new_circle->get_physical_body()->set_velocity(total_velocity);

        kill();
        other_circle->kill();
        return false;
    }

    return true;
}
