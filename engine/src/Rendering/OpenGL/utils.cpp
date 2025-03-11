#include "utils.h"

namespace Presto {
constexpr auto INFO_LOG_LENGTH = 512;

bool OpenGLUtils::ShaderCompiledCorrectly(GLuint shader) {
    GLint success = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        std::array<char, INFO_LOG_LENGTH> infoLog{};
        glGetShaderInfoLog(shader, INFO_LOG_LENGTH, nullptr, infoLog.data());

        PR_CORE_ERROR("Shader failed to compile: {}", infoLog.data());
    }

    return success == GL_TRUE;
}

bool OpenGLUtils::ShaderProgramLinkedCorrectly(GLuint shaderProgram) {
    GLint success{};
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLint logLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);

        std::string message("", logLength);
        glGetProgramInfoLog(shaderProgram, logLength, &logLength,
                            message.data());

        PR_CORE_ERROR("Shader program failed to link: {}", message);
    }

    return success != 0;
};
GLenum OpenGLUtils::PrestoDrawModeToOpenGLDrawMode(MeshDrawMode mode) {
    switch (mode) {
        case MeshDrawMode::POINTS:
            return GL_POINTS;
        case MeshDrawMode::LINES:
            return GL_LINES;
        case MeshDrawMode::LINE_STRIP:
            return GL_LINE_STRIP;
        case MeshDrawMode::TRIANGLES:
            return GL_TRIANGLES;
        case MeshDrawMode::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
    }
}
}  // namespace Presto
