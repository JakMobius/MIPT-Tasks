//
// Created by Артем on 15.09.2021.
//

#include "circle_body.hpp"
#include "../contacts/circle_circle_contact.hpp"
#include "../contacts/circle_plane_contact.hpp"

namespace Physics {
Contact* CircleBody::intersects(const RigidBody* other) {
    switch(other->get_type()) {
        case ColliderType::circle:
            return CircleCircleContact::get_contact(this, (CircleBody*) other);
        case ColliderType::plane:
            return CirclePlaneContact::get_contact(this, (PlaneBody*) other);
        default:
            assert(false);
    }
}
}