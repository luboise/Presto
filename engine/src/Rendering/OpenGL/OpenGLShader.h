#pragma once

#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Shader.h"

#include <map>

#include <GL/glew.h>

namespace Presto {
class OpenGLShader final : public Shader {
   public:
    using shader_property_key_t = PR_STRING_ID;

    using opengl_shader_ptr_t = std::shared_ptr<OpenGLShader>;

    OpenGLShader();

    void use() const override;

    void setGlobalUniforms(GlobalUniforms uniforms) override;
    void setObjectUniforms(ObjectUniforms uniforms) override;

    void setUniform(uniform_name_t property, Presto::uint32_t value) override;

    void setUniform(uniform_name_t property, Presto::int32_t value) override;
    void setUniform(uniform_name_t property, Presto::float32_t value) override;

    void setUniform(uniform_name_t property, Presto::vec2 value) override;
    void setUniform(uniform_name_t property, Presto::vec3 value) override;
    void setUniform(uniform_name_t property, Presto::vec4 value) override;
    void setUniform(uniform_name_t property, Presto::mat4 value) override;

    inline void setMat4(uniform_name_t property, float* value);

    OpenGLShader& setShader(const AssetArg& shaderPath, ShaderStage type);
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
