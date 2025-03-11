#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/RenderTypes.h"

namespace Presto::OpenGLUtils {

GLenum PrestoDrawModeToOpenGLDrawMode(MeshDrawMode mode);

bool ShaderCompiledCorrectly(GLuint shader);
bool ShaderProgramLinkedCorrectly(GLuint shaderProgram);

}  // namespace Presto::OpenGLUtils
