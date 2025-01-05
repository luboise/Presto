#include "utils.h"

namespace PrestoCore {
    constexpr auto INFO_LOG_LENGTH = 512;

    bool OpenGLUtils::ShaderCompiledCorrectly(GLuint shader) {
        GLint success = 0;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success == 0) {
            std::array<char, INFO_LOG_LENGTH> infoLog{};
            glGetShaderInfoLog(shader, INFO_LOG_LENGTH, nullptr,
                               infoLog.data());

            PR_CORE_ERROR("Shader failed to compile: {}", infoLog.data());
        }

        return success == GL_TRUE;
    }
}  // namespace PrestoCore
