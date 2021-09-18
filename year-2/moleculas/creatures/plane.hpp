#pragma once

#include "creature.hpp"
#include "../fixtures/circle_fixture.hpp"

class Plane : public Creature {

public:
    Plane(const Vec2d& center, PlaneOrientation orientation): Creature() {
        fixture = new PlaneFixture(center, orientation);
    };

    ~Plane() override { delete fixture; }

    void draw(DrawingContext* ctx) override {
        auto plane_fixture = (PlaneFixture*)fixture;
        if(plane_fixture->get_orientation() == PlaneOrientation::vertical) {
            ctx->draw_rect({plane_fixture->get_position().x, 0}, {1, ctx->get_height()});
        } else {
            ctx->draw_rect({0, plane_fixture->get_position().y}, {ctx->get_width(), 1});
        }
    }
};