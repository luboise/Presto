export module presto.common:math;

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

export namespace Pr {
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
}  // namespace Pr

// #include "glm/fwd.hpp"
