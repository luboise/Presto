#pragma once

#include "Presto/Collision/Shapes.h"
#include "Presto/Objects/Component.h"

#include "Presto/Physics/Force.h"
#include "Presto/Utils/SubTypeMap.h"

namespace Presto {

class RenderingManager;

class PRESTO_API RigidBodyComponent : public Component {
   public:
    RigidBodyComponent() = default;

    void addVelocity(vec3 vel);
    void addAngularVelocity(vec3 angularVel);
    void addForce(Force force);

    [[nodiscard]] bool collidingWith(const RigidBodyComponent& other) const;

    struct PhysicsMovement {
        vec3 pos_offset;
        vec3 angular_offset;
    };

    [[nodiscard]] Force calculateMovement() const;

   private:
    Force force_;
    SubTypeMap<CollisionShape> colliders_;
};

}  // namespace Presto
