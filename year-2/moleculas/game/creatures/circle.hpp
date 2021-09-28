#pragma once

class Circle;

#include "game_object.hpp"
#include "../../physics-engine/bodies/circle_body.hpp"
#include "../../visual-engine/visual_component.hpp"
#include "super_circle.hpp"

class Circle : public GameObject {
    VisualComponentCircle* visual_component;

public:
    Circle(const Vec2d& center, double radius, double mass = 1): GameObject(EntityType::small_ball) {
        body = new Physics::CircleBody(this, center, radius, mass);
        visual_component = new VisualComponentCircle(this);
    };

    ~Circle() {
        delete body;
        delete visual_component;
    }

    void draw(DrawingContext* ctx) override {
        auto circle_fixture = (Physics::CircleBody*)body;
        visual_component->set_radius(circle_fixture->get_radius());
        visual_component->set_position(body->get_position());
        visual_component->draw(ctx);
    }

    bool on_collision(Entity *other) override;
};