#pragma once

class SuperCircle;

#include "circle.hpp"

class SuperCircle : public GameObject {
    int eaten_balls = 0;
    double eaten_energy = 0;
    VisualComponentSquare* visual_component;

public:
    SuperCircle(const Vec2d& center): GameObject(EntityType::super_ball) {
        body = new Physics::CircleBody(this, center, 1, 1);
        visual_component = new VisualComponentSquare(this);
    };

    void draw(DrawingContext* ctx) override {
        auto circle_fixture = (Physics::CircleBody*)body;
        visual_component->set_size({circle_fixture->get_radius(), circle_fixture->get_radius()});
        visual_component->set_position(body->get_position());
        visual_component->draw(ctx);
    }

    int get_eaten_balls() { return eaten_balls; }
    void eat_ball(Circle* ball);

    double get_eaten_energy() { return eaten_energy; }

    bool on_collision(Entity *other) override;

    bool explode_with_other_circle(SuperCircle* other);

    bool eat_other_circle(Circle* other);
};