#include "presto/platform.h"

#include "presto/objects/component.h"

#include "presto/core.h"
#include "presto/physics.h"

namespace Presto {

class RenderingManager;

class PRESTO_API RigidBodyComponent : public Component {
   public:
    RigidBodyComponent() = default;

    [[nodiscard]] Presto::vec3 velocity() const;
    void addVelocity(Presto::vec3 vel);
    void setVelocity(Presto::vec3 vel);

    [[nodiscard]] Presto::vec3 angularVelocity() const;
    void addAngularVelocity(Presto::vec3 angularVel);
    void setAngularVelocity(Presto::vec3 angularVel);

    void addForce(Force force);

    [[nodiscard]] float drag() const;
    void setDrag(float);

    [[nodiscard]] const Force& force() const;
    void setForce(Force);

    [[nodiscard]] bool collidingWith(const RigidBodyComponent& other) const;

    struct PhysicsMovement {
        Presto::vec3 pos_offset;
        Presto::vec3 angular_offset;
    };

    [[nodiscard]] Force calculateMovement() const;

   private:
    Force force_;
    float drag_{0.5F};
};

}  // namespace Presto
