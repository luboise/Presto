module;
#include "presto/module.h"

export module presto.internal.managers.physics;
import presto.objects;

export namespace Presto {

class PhysicsManager final : public Module<PhysicsManager> {
    MODULE_FUNCTIONS(PhysicsManager);

    friend void Entity::checkNewComponent(GenericComponentPtr);

   public:
    void update() override;
    void clear();

    void addPersistentForce(Force);

    struct PhysicsPairing {
        Entity* entity;
        ComponentPtr<RigidBodyComponent> body;
    };

   private:
    explicit PhysicsManager();
    ~PhysicsManager() override;

    void addPairing(const PhysicsPairing&);

    std::vector<PhysicsPairing> pairings_;
    std::vector<Force> persistentForces_;
};

}  // namespace Presto
