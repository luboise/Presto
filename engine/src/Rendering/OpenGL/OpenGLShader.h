#pragma once

#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Shader.h"

#include <map>

#include <GL/glew.h>

namespace Presto {
class OpenGLShader final : public Shader {
    static constexpr auto INVALID_SHADER_ID = 0;

   public:
    using shader_property_key_t = std::string;

    using opengl_shader_ptr_t = std::shared_ptr<OpenGLShader>;

    OpenGLShader();

    void use() const override;

    void setGlobalUniforms(GlobalUniforms uniforms) override;
    void setObjectUniforms(ObjectUniforms uniforms) override;

    void setUniform(uniform_name_t property, std::uint8_t value) override;

    void setUniform(uniform_name_t property, int value) override;
    void setUniform(uniform_name_t property, float value) override;
    void setUniform(uniform_name_t property, Presto::mat4 value) override;

    inline void setMat4(uniform_name_t property, float* value);

    void setTexture(uniform_name_t property, Texture value) override;

    OpenGLShader& setShader(const AssetPath& shaderPath, ShaderStage type);
    OpenGLShader& setShader(const char* data, ShaderStage type);

    void linkShaderProgram();

    [[nodiscard]] GLuint getShaderProgram() const { return shaderProgram_; }

   private:
    GLuint shaderProgram_;
    GLuint vertexShader_{INVALID_SHADER_ID};
    GLuint fragmentShader_{INVALID_SHADER_ID};

    std::map<shader_property_key_t, GLint> propertyMap_;
};

}  // namespace Presto
