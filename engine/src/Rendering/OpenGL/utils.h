#pragma once

#include <GL/glew.h>

namespace Presto::OpenGLUtils {
    bool ShaderCompiledCorrectly(GLuint shader);
    bool ShaderProgramLinkedCorrectly(GLuint shaderProgram);
}  // namespace Presto::OpenGLUtils
