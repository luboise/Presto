#include "internal/rendering.h"
import :shader;

namespace Pr {
// TODO: Fix this up to be using a reference of some kind
ShaderInfo Shader::getInfo() const {
    return {.attributes = attributes_, .uniforms = uniforms_};
};

}  // namespace Pr
