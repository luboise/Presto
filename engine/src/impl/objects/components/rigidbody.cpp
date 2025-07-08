module presto.objects.components.rigidbody;

import glm;
import glm;

namespace Pr {

[[nodiscard]] Force RigidBodyComponent::calculateMovement() const {
    return force_;
}

void RigidBodyComponent::addVelocity(Pr::vec3 vel) {
    this->setVelocity(velocity() + vel);
};

void RigidBodyComponent::addAngularVelocity(Pr::vec3 angularVel) {
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

Pr::vec3 RigidBodyComponent::velocity() const { return force_.velocity; }
Pr::vec3 RigidBodyComponent::angularVelocity() const {
    return force_.angular_velocity;
};

void RigidBodyComponent::setVelocity(Pr::vec3 vel) { force_.velocity = vel; };

void RigidBodyComponent::setAngularVelocity(Pr::vec3 angularVel) {
    force_.angular_velocity = angularVel;
};

}  // namespace Pr
