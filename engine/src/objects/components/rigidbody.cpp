#include "presto/objects/components.h"
import :rigidbody;

namespace Pr {

[[nodiscard]] Force RigidBodyComponent::calculateMovement() const {
    return force_;
}

void RigidBodyComponent::addVelocity(vec3 vel) {
    this->setVelocity(velocity() + vel);
};

void RigidBodyComponent::addAngularVelocity(vec3 angularVel) {
    this->setAngularVelocity(angularVelocity() + angularVel);
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

const Force& RigidBodyComponent::force() const { return force_; };
float RigidBodyComponent::drag() const { return drag_; };

void RigidBodyComponent::setForce(Force f) { force_ = f; };

void RigidBodyComponent::setDrag(float drag) { drag_ = drag; };

vec3 RigidBodyComponent::velocity() const { return force_.velocity; }
vec3 RigidBodyComponent::angularVelocity() const {
    return force_.angular_velocity;
};

void RigidBodyComponent::setVelocity(vec3 vel) { force_.velocity = vel; };

void RigidBodyComponent::setAngularVelocity(vec3 angularVel) {
    force_.angular_velocity = angularVel;
};

}  // namespace Pr
