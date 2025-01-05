#include "Presto/Components/RenderableProps.h"

namespace Presto {
    RenderableProps::RenderableProps()
        : _translation(0), _yawPitchRoll(0), _scale(1) {}

    RenderableProps::RenderableProps(vec3 translation, vec3 rotation,
                                     vec3 scale)
        : _translation(translation), _yawPitchRoll(rotation), _scale(scale) {}

    mat4 RenderableProps::getModelMatrix(vec3 offset, glm::float32 scale) {
        return getModelMatrix(offset, vec3(0.0f), scale);
    }

    mat4 RenderableProps::getModelMatrix(vec3 offset, vec3 yawPitchRoll,
                                         glm::float32 scale) {
        mat4 model(1.0f);

        model = glm::rotate(model, yawPitchRoll.y, vec3(1, 0, 0));
        model = glm::rotate(model, yawPitchRoll.x, vec3(0, 1, 0));
        model = glm::rotate(model, yawPitchRoll.z, vec3(0, 0, 1));

        mat4 scaleMatrix = {
            {scale, 0, 0, 0},
            {0, scale, 0, 0},
            {0, 0, scale, 0},
            {0, 0, 0, 1},
        };
        model = scaleMatrix * model;

        return model;
    }

    // vec3 ModelComponent::getProjected(vec3 yawPitchRoll) const {
    //     return getProjected(yawPitchRoll, vec3(0, 0, 10.0f));
    // }

    // vec3 ModelComponent::getProjected(vec3 yawPitchRoll,
    //                                        vec3 cameraPos) const {
    //     return getProjected(yawPitchRoll, cameraPos, 1.0);
    // }

    // vec3 ModelComponent::getProjected(vec3 yawPitchRoll,
    //                                        vec3 cameraPos,
    //                                        glm::float32 scale) const {
    //     auto fovYDeg = 90;
    //     VkExtent2D viewport{1280, 720};

    //     auto mvm = getModelMatrix(cameraPos, yawPitchRoll, scale);
    //     auto pm = getProjectionMatrix(fovYDeg, viewport);

    //     auto projected = pm * mvm * glm::vec4(this->pos, 1);

    //     // Normalise to -1, 1
    //     auto normalised = vec3(projected) / projected.w;
    //     return normalised;
    // }

}  // namespace Presto
