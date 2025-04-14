#include "Presto/Objects/Components/Physics/RigidBodyComponent.h"

namespace Presto {

[[nodiscard]] Force RigidBodyComponent::calculateMovement() const {
    return force_;
}

void RigidBodyComponent::addVelocity(vec3 vel) { force_.velocity += vel; };

void RigidBodyComponent::addAngularVelocity(vec3 angularVel) {
    force_.angular_velocity += angularVel;
};

void RigidBodyComponent::addForce(Force force) { force_ += force; }

bool RigidBodyComponent::collidingWith(const RigidBodyComponent& other) const {
    /*
for (auto a : colliders_) {
    for (auto b : other.colliders_) {
        if (Intersects(a, b)) {
            return true;
        }
    }
}
    */
    return false;
}

}  // namespace Presto
