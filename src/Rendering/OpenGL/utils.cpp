#include "utils.h"

namespace Presto {
    bool OpenGLUtils::ShaderCompiledCorrectly(GLuint shader) {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        return success == GL_TRUE;
    }
}  // namespace Presto
