#include "Presto/Components/Physics/RigidBody.h"

namespace Presto {
    [[nodiscard]] RigidBody::PhysicsMovement RigidBody::calculateMovement()
        const {
        return {.pos_offset = velocity_, .angular_offset = angularVelocity_};
    };

    void RigidBody::addVelocity(vec3 vel) { velocity_ += vel; };

}  // namespace Presto
