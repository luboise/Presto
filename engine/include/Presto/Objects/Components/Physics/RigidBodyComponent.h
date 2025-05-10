#pragma once

#include "Presto/Objects/Component.h"

#include "Presto/Physics/Force.h"

namespace Presto {

class RenderingManager;

class PRESTO_API RigidBodyComponent : public Component {
   public:
    RigidBodyComponent() = default;

    [[nodiscard]] vec3 velocity() const;
    void addVelocity(vec3 vel);
    void setVelocity(vec3 vel);

    [[nodiscard]] vec3 angularVelocity() const;
    void addAngularVelocity(vec3 angularVel);
    void setAngularVelocity(vec3 angularVel);

    void addForce(Force force);

    [[nodiscard]] float drag() const;
    void setDrag(float);

    [[nodiscard]] const Force& force() const;
    void setForce(Force);

    [[nodiscard]] bool collidingWith(const RigidBodyComponent& other) const;

    struct PhysicsMovement {
        vec3 pos_offset;
        vec3 angular_offset;
    };

    [[nodiscard]] Force calculateMovement() const;

   private:
    Force force_;
    float drag_{0.5F};
    // TypeMap<CollisionShape> colliders_;
};

}  // namespace Presto
