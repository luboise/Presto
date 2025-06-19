export module presto.internal.rendering.opengl:shader;

import presto.internal.rendering;

#include <map>

#include <GL/glew.h>

namespace Presto {
class OpenGLShader final : public Shader {
   public:
    using shader_property_key_t = Presto::string;

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

class OpenGLVAO {
    using attribute_offset_t = std::int64_t;

   public:
    OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
              const PipelineStructure& structure);
    OpenGLVAO();
    // OpenGLVAO();
    ~OpenGLVAO();

    void bind() const;

    OpenGLVAO& setAttribs(const PipelineStructure&);
    void finalise();
    [[nodiscard]] bool finalised() const { return finalised_; }

    OpenGLVAO(const OpenGLVAO&) = delete;
    OpenGLVAO& operator=(const OpenGLVAO&) = delete;
    OpenGLVAO(OpenGLVAO&&) = delete;
    OpenGLVAO& operator=(OpenGLVAO&&) = delete;

   private:
    void setAttribPointer(const PipelineAttribute& attribute, GLsizei stride,
                          attribute_offset_t offset);

    bool finalised_{false};

    GLuint vao_{};
};

}  // namespace Presto
