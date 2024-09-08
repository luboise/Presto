#include "ModelComponent.h"

namespace Presto {

    glm::mat4 ModelComponent::getModelMatrix(glm::vec3 offset,
                                             glm::float32 scale) {
        return getModelMatrix(offset, glm::vec3(0.0f), scale);
    }

    glm::mat4 ModelComponent::getModelMatrix(glm::vec3 offset,
                                             glm::vec3 yawPitchRoll,
                                             glm::float32 scale) {
        glm::mat4 model(1.0f);

        model = glm::rotate(model, yawPitchRoll.y, glm::vec3(1, 0, 0));
        model = glm::rotate(model, yawPitchRoll.x, glm::vec3(0, 1, 0));
        model = glm::rotate(model, yawPitchRoll.z, glm::vec3(0, 0, 1));

        glm::mat4 scaleMatrix = {
            {scale, 0, 0, 0},
            {0, scale, 0, 0},
            {0, 0, scale, 0},
            {0, 0, 0, 1},
        };
        model = scaleMatrix * model;

        return model;
    }

    // glm::vec3 ModelComponent::getProjected(glm::vec3 yawPitchRoll) const {
    //     return getProjected(yawPitchRoll, glm::vec3(0, 0, 10.0f));
    // }

    // glm::vec3 ModelComponent::getProjected(glm::vec3 yawPitchRoll,
    //                                        glm::vec3 cameraPos) const {
    //     return getProjected(yawPitchRoll, cameraPos, 1.0);
    // }

    // glm::vec3 ModelComponent::getProjected(glm::vec3 yawPitchRoll,
    //                                        glm::vec3 cameraPos,
    //                                        glm::float32 scale) const {
    //     auto fovYDeg = 90;
    //     VkExtent2D viewport{1280, 720};

    //     auto mvm = getModelMatrix(cameraPos, yawPitchRoll, scale);
    //     auto pm = getProjectionMatrix(fovYDeg, viewport);

    //     auto projected = pm * mvm * glm::vec4(this->pos, 1);

    //     // Normalise to -1, 1
    //     auto normalised = glm::vec3(projected) / projected.w;
    //     return normalised;
    // }

}  // namespace Presto