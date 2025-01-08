#pragma once

#include <glm/glm.hpp>

namespace Presto {
    class RenderingManager;

    class PRESTO_API RenderableProps {
       public:
        static mat4 getModelMatrix(vec3 offset, vec3 yawPitchRoll,
                                   glm::float32 scale);
        static mat4 getModelMatrix(vec3 offset, glm::float32 scale);

        void setTranslation(vec3 translation);
        void setRotation(vec3 yawPitchRoll);
        void setScale(vec3 scale);

        [[nodiscard]] vec3 getModelView() const;
        [[nodiscard]] vec3 getScale() const;
        [[nodiscard]] vec3 getTranslation() const;

        // Returns vec3(yaw,pitch,roll)
        [[nodiscard]] vec3 getRotation() const;

        [[nodiscard]] vec3 getYaw() const;
        [[nodiscard]] vec3 getPitch() const;
        [[nodiscard]] vec3 getRoll() const;

       private:
        RenderableProps();
        RenderableProps(vec3 translation, vec3 rotation, vec3 scale);

        vec3 _translation;
        vec3 _yawPitchRoll;
        vec3 _scale;
        // mat4 _modelView;
        friend class RenderingManager;
    };
}  // namespace Presto
