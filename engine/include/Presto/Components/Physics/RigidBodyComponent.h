#pragma once

#include "Presto/Objects/Component.h"

#include "Presto/Physics/Force.h"

namespace Presto {
    class RenderingManager;

    class PRESTO_API RigidBodyComponent : public Component {
       public:
        RigidBodyComponent() = default;

        void addVelocity(vec3 vel);
        void addAngularVelocity(vec3 angularVel);
        void addForce(Force force);

        struct PhysicsMovement {
            vec3 pos_offset;
            vec3 angular_offset;
        };

        [[nodiscard]] Force calculateMovement() const;

       private:
        Force force_;
    };
}  // namespace Presto
