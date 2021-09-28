#pragma once

#include "../components/component.hpp"
#include "drawing_context.hpp"
#include "../physics-engine/bodies/plane_body.hpp"

class VisualComponentDelegate : public virtual ComponentDelegate {

};

class VisualComponent : public Component<VisualComponentDelegate> {
protected:
    Vec2d position {0, 0};
public:
    explicit VisualComponent(VisualComponentDelegate* entity): Component(entity) {};
    virtual void draw(DrawingContext* ctx) = 0;

    void set_position(const Vec2d& position) { this->position = position; }
};

class VisualComponentCircle : public VisualComponent {
protected:
    double radius = 1;
public:
    explicit VisualComponentCircle(VisualComponentDelegate* entity): VisualComponent(entity) {};

    void draw(DrawingContext* ctx) override {
        ctx->set_color(1, 1, 1, 1);
        ctx->draw_circle(position, radius);
    }

    void set_radius(double radius) { this->radius = radius; };
};

class VisualComponentSquare : public VisualComponent {
protected:
    Vec2d size = {1, 1};
public:
    explicit VisualComponentSquare(VisualComponentDelegate* entity): VisualComponent(entity) {};

    void draw(DrawingContext* ctx) override {
        ctx->set_color(1, 0.5, 0.5, 1);
        ctx->draw_rect(position, size);
    }

    void set_size(const Vec2d& size) { this->size = size; };
};

class VisualComponentPlane : public VisualComponent {
protected:
    Physics::PlaneOrientation orientation;
public:
    explicit VisualComponentPlane(VisualComponentDelegate* entity): VisualComponent(entity) {};

    void draw(DrawingContext* ctx) override {
        ctx->set_color(0, 1, 0, 1);
        if(orientation == Physics::PlaneOrientation::vertical) {
            ctx->draw_rect({position.x, 0}, {1, ctx->get_height()});
        } else {
            ctx->draw_rect({0, position.y}, {ctx->get_width(), 1});
        }
    }

    void set_orientation(Physics::PlaneOrientation orientation) { this->orientation = orientation; }
};