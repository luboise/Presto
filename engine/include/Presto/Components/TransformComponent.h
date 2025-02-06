#pragma once

#include "Presto/Objects/Component.h"

namespace Presto {
class RenderingManager;

class PRESTO_API TransformComponent : public Component {
   public:
    TransformComponent() = default;
    TransformComponent(vec3 translation, vec3 rotation, vec3 scale);

    static mat4 getModelMatrix(vec3 offset, vec3 yawPitchRoll,
                               glm::float32 scale) {
        return getModelMatrix(offset, yawPitchRoll, vec3(scale));
    };

    static mat4 getModelMatrix(vec3 offset, vec3 yawPitchRoll, vec3 scale);

    TransformComponent& translate(vec3 translation);
    TransformComponent& translate(double x = 0, double y = 0, double z = 0) {
        return this->translate(vec3{x, y, z});
    }

    TransformComponent& rotate(vec3 rotation);
    TransformComponent& rotate(double x = 0, double y = 0, double z = 0);

    TransformComponent& setTranslation(vec3 translation);
    TransformComponent& setRotation(vec3 yawPitchRoll);

    TransformComponent& setScale(float);
    TransformComponent& setScale(vec3 scale);

    [[nodiscard]] vec3 getScale() const;
    [[nodiscard]] vec3 getTranslation() const;

    [[nodiscard]] vec3 getPosition() const;

    // Returns (yaw,pitch,roll)
    [[nodiscard]] vec3 getRotation() const;

    [[nodiscard]] vec3 getYaw() const;
    [[nodiscard]] vec3 getPitch() const;
    [[nodiscard]] vec3 getRoll() const;

    // void setRounding(bool x, bool y, bool z) { useRounding_ = {x, y, z};
    // }

   private:
    [[nodiscard]] inline mat4 getModelView() const {
        return getModelMatrix(translation_, yawPitchRoll_, scale_);
    };

    vec3 translation_;
    vec3 yawPitchRoll_;
    vec3 scale_;

    // std::array<bool, 3> useRounding_{false};

    // void round();

    // mat4 _modelView;
    friend class RenderingManager;
};
}  // namespace Presto
