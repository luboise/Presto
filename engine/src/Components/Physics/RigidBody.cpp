#include "Presto/Components/Physics/RigidBody.h"

namespace Presto {
    [[nodiscard]] Force RigidBody::calculateMovement() const { return force_; }

    void RigidBody::addVelocity(vec3 vel) { force_.velocity += vel; };

    void RigidBody::addAngularVelocity(vec3 angularVel) {
        force_.angular_velocity += angularVel;
    };

    void RigidBody::addForce(Force force) { force_ += force; }

}  // namespace Presto
