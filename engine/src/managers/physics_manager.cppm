module;
#include "presto/module.h"

export module presto.internal.managers.physics_manager;

import std;

import presto.internal.managers.manager;
import presto.physics;

import presto.objects;
import presto.objects.components.rigidbody;

export namespace Pr {

class PhysicsManager final : public Module<PhysicsManager> {
    MODULE_FUNCTIONS(PhysicsManager);

    // friend void Entity::checkNewComponent(GenericComponentPtr);

   public:
    void update() override;
    void clear();

    void addPersistentForce(Force);

    struct PhysicsPairing {
        Entity* entity;
        ComponentPtr<RigidBodyComponent> body;
    };

    void addPairing(const PhysicsPairing&);

   private:
    explicit PhysicsManager();
    ~PhysicsManager() override;

    std::vector<PhysicsPairing> pairings_;
    std::vector<Force> persistentForces_;
};

}  // namespace Pr
