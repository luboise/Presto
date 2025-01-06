#include "Presto/Components/Transform.h"
#include "Presto/Modules/EntityManager.h"

namespace Presto {
    mat4 Transform::getModelMatrix(vec3 offset, vec3 yawPitchRoll, vec3 scale) {
        mat4 model(1.0F);

        model = glm::rotate(model, yawPitchRoll.y, vec3(1, 0, 0));
        model = glm::rotate(model, yawPitchRoll.x, vec3(0, 1, 0));
        model = glm::rotate(model, yawPitchRoll.z, vec3(0, 0, 1));
        model = glm::scale(model, scale);

        return model;
    }

    Transform* Transform::New() {
        auto& em{EntityManager::Get()};
        return em.newComponent<Transform>();
    }
}  // namespace Presto
