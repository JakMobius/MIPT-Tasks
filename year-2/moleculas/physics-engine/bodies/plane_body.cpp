//
// Created by Артем on 15.09.2021.
//

#include "plane_body.hpp"
#include "circle_body.hpp"
#include "../contacts/circle_plane_contact.hpp"

namespace Physics {
Contact* PlaneBody::intersects(const RigidBody* other) {
    switch(other->get_type()) {
        case ColliderType::circle:
            return CirclePlaneContact::get_contact((CircleBody*) other, this);
        case ColliderType::plane:
            return nullptr;
        default:
            assert(false);
    }
}
}