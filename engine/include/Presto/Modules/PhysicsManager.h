#pragma once

#include "Module.h"

#include "Presto/Components/Physics/RigidBody.h"
#include "Presto/Objects/Entity.h"

namespace Presto {
    class PRESTO_API PhysicsManager : public Module<PhysicsManager> {
        friend void Entity::checkNewComponent(Component*);

       public:
        static void init();

        void update() override;
        void clear();

        static void shutdown();

        PhysicsManager(const PhysicsManager&) = delete;
        PhysicsManager(PhysicsManager&&) = delete;
        PhysicsManager& operator=(const PhysicsManager&) = delete;
        PhysicsManager& operator=(PhysicsManager&&) = delete;

        struct PhysicsPairing {
            Entity* entity;
            RigidBody* body;
        };

       private:
        explicit PhysicsManager() = default;

        void addPairing(const PhysicsPairing&);
        std::vector<PhysicsPairing> pairings_;
    };
}  // namespace Presto
