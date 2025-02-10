#include "Presto/Rendering/Shader.h"

namespace Presto {
// TODO: Fix this up to be using a reference of some kind
ShaderInfo Shader::getInfo() const {
    return {.attributes = attributes_, .uniforms = uniforms_};
};
}  // namespace Presto
