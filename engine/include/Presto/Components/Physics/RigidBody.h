#pragma once

#include "Presto/Objects/Component.h"

namespace Presto {
    class RenderingManager;

    class PRESTO_API RigidBody : public Component {
       public:
        RigidBody() = default;

        void addVelocity(vec3 vel);

        struct PhysicsMovement {
            vec3 pos_offset;
            vec3 angular_offset;
        };

        [[nodiscard]] PhysicsMovement calculateMovement() const;

       private:
        vec3 velocity_{0};
        vec3 angularVelocity_{0};
    };
}  // namespace Presto
