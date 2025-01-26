#include "Presto/Modules/PhysicsManager.h"

#include "Presto/Components/Transform.h"

#include "Presto/Runtime/Time.h"

#include "Presto/Components/Physics/RigidBody.h"
#include "Presto/Physics/Force.h"

namespace Presto {

    void PhysicsManager::init() {
        instance_ = std::unique_ptr<PhysicsManager>(new PhysicsManager());
    }

    void PhysicsManager::shutdown() { instance_->pairings_.clear(); }

    void PhysicsManager::update() {
        auto delta{Time::deltaSeconds()};

        for (auto& pairing : pairings_) {
            // Add all persistent forces
            for (const auto& persistent_force : persistentForces_) {
                pairing.body->addForce(persistent_force);
            }

            Force force{pairing.body->calculateMovement() * delta};

            pairing.entity->getComponent<Transform>()
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