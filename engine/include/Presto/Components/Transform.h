#pragma once

#include "Presto/Objects/Component.h"

namespace Presto {
    class RenderingManager;

    using glm::mat4;
    using glm::vec2;
    using glm::vec3;

    class PRESTO_API Transform : public Component {
       public:
        Transform() = default;
        Transform(vec3 translation, vec3 rotation, vec3 scale);

        static mat4 getModelMatrix(vec3 offset, vec3 yawPitchRoll,
                                   glm::float32 scale) {
            return getModelMatrix(offset, yawPitchRoll, vec3(scale));
        };

        static mat4 getModelMatrix(vec3 offset, vec3 yawPitchRoll, vec3 scale);

        Transform& translate(vec3 translation);
        Transform& translate(double x = 0, double y = 0, double z = 0) {
            return this->translate(vec3{x, y, z});
        }

        Transform& rotate(double x = 0, double y = 0, double z = 0);

        Transform& setTranslation(vec3 translation);
        Transform& setRotation(vec3 yawPitchRoll);
        Transform& setScale(vec3 scale);

        [[nodiscard]] vec3 getScale() const;
        [[nodiscard]] vec3 getTranslation() const;

        // Returns vec3(yaw,pitch,roll)
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

        void round();

        // mat4 _modelView;
        friend class RenderingManager;
    };
}  // namespace Presto
