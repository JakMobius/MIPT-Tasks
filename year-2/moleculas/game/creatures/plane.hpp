#pragma once

#include "game_object.hpp"
#include "../../physics-engine/bodies/plane_body.hpp"

class Plane : public GameObject {
    VisualComponentPlane* visual_component;

public:
    Plane(const Vec2d &center, Physics::PlaneOrientation orientation) : GameObject(EntityType::wall) {
        body = new Physics::PlaneBody(this, center, orientation);
        visual_component = new VisualComponentPlane(this);
    };

    void draw(DrawingContext* ctx) override {
        auto circle_fixture = (Physics::PlaneBody*)body;

        visual_component->set_orientation(circle_fixture->get_orientation());
        visual_component->set_position(body->get_position());
        visual_component->draw(ctx);
    }


};