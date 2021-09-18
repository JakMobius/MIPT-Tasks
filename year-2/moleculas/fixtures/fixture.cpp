#include "fixture.hpp"
#include "circle_fixture.hpp"

Contact* Fixture::intersects(const Fixture *other) {
    switch(type) {
        case ColliderType::circle:
            assert(&CircleFixture::intersects != &Fixture::intersects);
            return ((CircleFixture*) this)->intersects(other);
        case ColliderType::plane:
            assert(&PlaneFixture::intersects != &Fixture::intersects);
            return ((PlaneFixture*) this)->intersects(other);
        default: assert(false);
    }
}

void Fixture::add_tick_velocity(const Vec2d &velocity) {
    tick_velocity_modification += velocity;
}

void Fixture::update_velocity() {
    velocity += tick_velocity_modification;
    tick_velocity_modification *= 0;
}

double Fixture::get_mass() const {
    return mass;
}
