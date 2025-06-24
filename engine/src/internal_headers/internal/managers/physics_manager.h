#include "presto/core/manager.h"

#include "presto/physics/types.h"

#include "presto/objects.h"

namespace Pr {

class PhysicsManager final : public Module<PhysicsManager> {
    MODULE_FUNCTIONS(PhysicsManager);

    friend void Entity::checkNewComponent(GenericComponentPtr);

   public:
    void update() override;
    void clear();

    void addPersistentForce(Force);

    struct PhysicsPairing {
        Entity* entity;
        Ptr<RigidBodyComponent> body;
    };

   private:
    explicit PhysicsManager();
    ~PhysicsManager() override;

    void addPairing(const PhysicsPairing&);

    std::vector<PhysicsPairing> pairings_;
    std::vector<Force> persistentForces_;
};

}  // namespace Pr
