#include "Presto/Modules/PhysicsManager.h"
#include "Presto/Components/Transform.h"

namespace Presto {

    void PhysicsManager::init() {
        instance_ = std::unique_ptr<PhysicsManager>(new PhysicsManager());
    }

    void PhysicsManager::shutdown() { instance_->pairings_.clear(); }

    void PhysicsManager::update() {
        for (auto& pairing : pairings_) {
            auto calculations{pairing.body->calculateMovement()};

            pairing.entity->getComponent<Transform>()
                ->translate(calculations.pos_offset)
                .rotate(calculations.angular_offset);
        }
    }
    void PhysicsManager::addPairing(const PhysicsPairing& pairing) {
        pairings_.push_back(pairing);
    };
}  // namespace Presto
