module;
#include "presto/platform.h"

export module presto.objects.components.rigidbody;
import presto.objects.component;

import presto.core;
import presto.physics;

export namespace Pr {

class RenderingManager;

class PRESTO_API RigidBodyComponent : public Component {
   public:
    RigidBodyComponent() = default;

    [[nodiscard]] Pr::vec3 velocity() const;
    void addVelocity(Pr::vec3 vel);
    void setVelocity(Pr::vec3 vel);

    [[nodiscard]] Pr::vec3 angularVelocity() const;
    void addAngularVelocity(Pr::vec3 angularVel);
    void setAngularVelocity(Pr::vec3 angularVel);

    void addForce(Force force);

    [[nodiscard]] float drag() const;
    void setDrag(float);

    [[nodiscard]] const Force& force() const;
    void setForce(Force);

    [[nodiscard]] bool collidingWith(const RigidBodyComponent& other) const;

    struct PhysicsMovement {
        Pr::vec3 pos_offset;
        Pr::vec3 angular_offset;
    };

    [[nodiscard]] Force calculateMovement() const;

   private:
    Force force_;
    float drag_{0.5F};
};

}  // namespace Pr
