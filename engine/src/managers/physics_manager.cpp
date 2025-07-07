module presto.internal.managers.physics_manager;

import presto.ext.glm;

import presto.runtime.time;
import presto.objects.components;

namespace Pr {

PhysicsManager::PhysicsManager() = default;

PhysicsManager::~PhysicsManager() { pairings_.clear(); }

void PhysicsManager::update() {
    auto delta{static_cast<float>(Time::deltaSeconds())};

    // Add all persistent forces
    for (PhysicsPairing& pairing : pairings_) {
        for (const auto& persistent_force : persistentForces_) {
            pairing.body->addForce(persistent_force);
        }

        Force force{pairing.body->calculateMovement()};

        pairing.body->setForce(force / ((1 + (delta * pairing.body->drag()))));

        pairing.entity->getComponent<TransformComponent>()
            ->translate(force.velocity * delta)
            .rotate(force.angular_velocity * delta);
    }

    // Check collisions
    /*
for (Pr::size_t i = 0; i < pairings_.size(); i++) {
    ComponentPtr<RigidBodyComponent>& pairing_a{pairings_[i].body};
    for (Pr::size_t j = i + 1; j < pairings_.size(); j++) {
        ComponentPtr<RigidBodyComponent>& pairing_b{pairings_[j].body};
    }
}
    */
}

void PhysicsManager::addPairing(const PhysicsPairing& pairing) {
    pairings_.push_back(pairing);
};

void PhysicsManager::addPersistentForce(Force force) {
    persistentForces_.push_back(force);
};
}  // namespace Pr
