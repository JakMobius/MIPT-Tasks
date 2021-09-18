//
// Created by Артем on 15.09.2021.
//

#include "plane_fixture.hpp"
#include "circle_fixture.hpp"
#include "../contacts/circle_plane_contact.hpp"

Contact* PlaneFixture::intersects(const Fixture *other) {
    switch(other->get_type()) {
        case ColliderType::circle: return CirclePlaneContact::get_contact((CircleFixture*) other, this);
        case ColliderType::plane: return nullptr;
        default: assert(false);
    }
}