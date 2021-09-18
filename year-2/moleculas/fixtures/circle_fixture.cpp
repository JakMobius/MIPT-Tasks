//
// Created by Артем on 15.09.2021.
//

#include "circle_fixture.hpp"
#include "../contacts/circle_circle_contact.hpp"
#include "../contacts/circle_plane_contact.hpp"

Contact* CircleFixture::intersects(const Fixture *other) {
    switch(other->get_type()) {
        case ColliderType::circle: return CircleCircleContact::get_contact(this, (CircleFixture*) other);
        case ColliderType::plane: return CirclePlaneContact::get_contact(this, (PlaneFixture*) other);
        default: assert(false);
    }
}
