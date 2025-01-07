#include "RenderingUtils.h"

namespace Presto {

    constexpr int COLOUR_RATIO = 101;

    glm::vec3 RenderingUtils::getRandomRed(const double maxNonRed) {
        auto vec =
            glm::vec3(1.0F, ((rand() % COLOUR_RATIO) / 100.0) * maxNonRed,
                      ((rand() % COLOUR_RATIO) / 100.0) * maxNonRed);
        return vec;
    }

    glm::mat4 RenderingUtils::getProjectionMatrix(glm::float32 fovDeg,
                                                  uint16_t width,
                                                  uint16_t height,
                                                  glm::float32 near,
                                                  glm::float32 far) {
        float fovRad = glm::radians(fovDeg);
        glm::mat4 projection = glm::perspectiveFov(
            fovRad, glm::float32(width), glm::float32(height), near, far);

        return projection;
    }

}  // namespace Presto
