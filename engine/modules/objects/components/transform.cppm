module;
#include "presto/platform.h"

export module presto.objects.components.transform;

import presto.objects.base;
import presto.core;
import presto.math;

export namespace Pr {

class PRESTO_API TransformComponent : public Component {
   public:
    TransformComponent() = default;
    TransformComponent(Pr::vec3 translation, Pr::vec3 rotation, Pr::vec3 scale);

    static mat4 getModelMatrix(Pr::vec3 offset, Pr::vec3 yawPitchRoll,
                               Pr::float32_t scale) {
        return getModelMatrix(offset, yawPitchRoll, Pr::vec3(scale));
    };

    static mat4 getModelMatrix(Pr::vec3 offset, Pr::vec3 yawPitchRoll,
                               Pr::vec3 scale);

    TransformComponent& translate(Pr::vec3 translation);
    TransformComponent& translate(double x = 0, double y = 0, double z = 0) {
        return this->translate(Pr::vec3{x, y, z});
    }

    [[nodiscard]] const TransformData& data() const;

    TransformComponent& rotate(Pr::vec3 rotation);
    TransformComponent& rotate(double x = 0, double y = 0, double z = 0);

    TransformComponent& setTranslation(Pr::vec3 translation);
    TransformComponent& setRotation(Pr::vec3 yawPitchRoll);

    TransformComponent& setScale(float);
    TransformComponent& setScale(Pr::vec3 scale);

    [[nodiscard]] Pr::vec3 getScale() const;
    [[nodiscard]] Pr::vec3 getTranslation() const;

    [[nodiscard]] Pr::vec3 getPosition() const;

    [[nodiscard]] Quaternion getRotation() const;

    [[nodiscard]] Pr::vec3 getYaw() const;
    [[nodiscard]] Pr::vec3 getPitch() const;
    [[nodiscard]] Pr::vec3 getRoll() const;

    // void setRounding(bool x, bool y, bool z) { useRounding_ = {x, y, z};
    // }

   private:
    [[nodiscard]] mat4 getModelView() const {
        return transformData_.asModelMat();
    };

    TransformData transformData_;

    // std::array<bool, 3> useRounding_{false};

    // void round();

    // mat4 _modelView;
};
}  // namespace Pr
