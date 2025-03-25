#include "Modules/PhysicsManager.h"

#include <chrono/physics/ChSystemSMC.h>
#include <memory>

#include "Presto/Objects/Components/TransformComponent.h"
#include "Presto/Physics/Force.h"
#include "Presto/Runtime/Time.h"

using namespace chrono;

namespace Presto {

PhysicsManager::PhysicsManager() { system_ = std::make_unique<ChSystemSMC>(); };

PhysicsManager::~PhysicsManager() { pairings_.clear(); }

void PhysicsManager::update() {
    auto delta{Time::deltaSeconds()};

    for (auto& pairing : pairings_) {
        // Add all persistent forces
        for (const auto& persistent_force : persistentForces_) {
            pairing.body->addForce(persistent_force);
        }

        Force force{pairing.body->calculateMovement() * delta};

        pairing.entity->getComponent<TransformComponent>()
            ->translate(force.velocity)
            .rotate(force.angular_velocity);
    }
}
void PhysicsManager::addPairing(const PhysicsPairing& pairing) {
    pairings_.push_back(pairing);
};

void PhysicsManager::addPersistentForce(Force force) {
    persistentForces_.push_back(force);
};

}  // namespace Presto
