#include "presto/core/platform.h"

#include "presto/core.h"
#include "presto/math.h"
#include "presto/objects/component.h"

namespace Presto {
class RenderingManager;

class PRESTO_API TransformComponent : public Component {
   public:
    TransformComponent() = default;
    TransformComponent(Presto::vec3 translation, Presto::vec3 rotation,
                       Presto::vec3 scale);

    static mat4 getModelMatrix(Presto::vec3 offset, Presto::vec3 yawPitchRoll,
                               glm::float32 scale) {
        return getModelMatrix(offset, yawPitchRoll, Presto::vec3(scale));
    };

    static mat4 getModelMatrix(Presto::vec3 offset, Presto::vec3 yawPitchRoll,
                               Presto::vec3 scale);

    TransformComponent& translate(Presto::vec3 translation);
    TransformComponent& translate(double x = 0, double y = 0, double z = 0) {
        return this->translate(Presto::vec3{x, y, z});
    }

    [[nodiscard]] const TransformData& data() const;

    TransformComponent& rotate(Presto::vec3 rotation);
    TransformComponent& rotate(double x = 0, double y = 0, double z = 0);

    TransformComponent& setTranslation(Presto::vec3 translation);
    TransformComponent& setRotation(Presto::vec3 yawPitchRoll);

    TransformComponent& setScale(float);
    TransformComponent& setScale(Presto::vec3 scale);

    [[nodiscard]] Presto::vec3 getScale() const;
    [[nodiscard]] Presto::vec3 getTranslation() const;

    [[nodiscard]] Presto::vec3 getPosition() const;

    [[nodiscard]] Quaternion getRotation() const;

    [[nodiscard]] Presto::vec3 getYaw() const;
    [[nodiscard]] Presto::vec3 getPitch() const;
    [[nodiscard]] Presto::vec3 getRoll() const;

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
    friend class RenderingManager;
};
}  // namespace Presto
