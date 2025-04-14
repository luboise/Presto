#include "Modules/PhysicsManager.h"

#include "Presto/Objects/Components/TransformComponent.h"
#include "Presto/Physics/Force.h"
#include "Presto/Runtime/Time.h"

namespace Presto {

PhysicsManager::PhysicsManager() = default;

PhysicsManager::~PhysicsManager() { pairings_.clear(); }

void PhysicsManager::update() {
    auto delta{Time::deltaSeconds()};

    // Add all persistent forces
    for (PhysicsPairing& pairing : pairings_) {
        for (const auto& persistent_force : persistentForces_) {
            pairing.body->addForce(persistent_force);
        }

        Force force{pairing.body->calculateMovement() * delta};

        pairing.entity->getComponent<TransformComponent>()
            ->translate(force.velocity)
            .rotate(force.angular_velocity);
    }

    // Check collisions
    /*
for (std::size_t i = 0; i < pairings_.size(); i++) {
    ComponentPtr<RigidBodyComponent>& pairing_a{pairings_[i].body};
    for (std::size_t j = i + 1; j < pairings_.size(); j++) {
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

}  // namespace Presto
