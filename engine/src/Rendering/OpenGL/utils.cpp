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
}  // namespace Presto
