
namespace Presto::RenderingUtils {

    glm::vec3 getRandomRed(double);

    glm::mat4 getProjectionMatrix(glm::float32 fovDeg, uint16_t width,
                                  uint16_t height, glm::float32 near = 0.01F,
                                  glm::float32 far = 300.0F);
}  // namespace Presto::RenderingUtils
