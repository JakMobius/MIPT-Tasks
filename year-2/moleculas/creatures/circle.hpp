#pragma once

#include "creature.hpp"
#include "../fixtures/circle_fixture.hpp"

class Circle : public Creature {
public:
    Circle(const Vec2d& center, double radius, double mass = 1): Creature() {
        fixture = new CircleFixture(center, radius, mass);
    };

    ~Circle() override { delete fixture; }

    void draw(DrawingContext* ctx) override {
        auto circle_fixture = (CircleFixture*)fixture;
//        ctx->set_color(fabs(fixture->get_velocity().x) / 8.4 + 0.5, fabs(fixture->get_velocity().y) / 8.4 + 0.5, 0, 1);
        ctx->draw_circle(circle_fixture->get_position(), (float)circle_fixture->radius);
    }
};