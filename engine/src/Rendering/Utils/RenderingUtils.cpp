#include "RenderingUtils.h"

namespace Presto {
glm::mat4 RenderingUtils::getProjectionMatrix(glm::float32 fovDeg,
                                              uint16_t width, uint16_t height,
                                              glm::float32 near,
                                              glm::float32 far) {
    float fovRad = glm::radians(fovDeg);
    glm::mat4 projection = glm::perspectiveFov(fovRad, static_cast<glm::float32>(width),
                                               static_cast<glm::float32>(height), near, far);

    return projection;
}

}  // namespace Presto
