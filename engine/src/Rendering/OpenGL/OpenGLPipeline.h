#pragma once

#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Rendering/OpenGL/OpenGLShader.h"

namespace Presto {
using ShaderPtr = OpenGLShader::opengl_shader_ptr_t;

class OpenGLPipeline final : public Pipeline {
    friend class OpenGLRenderer;
    friend class OpenGLDrawManager;

    explicit OpenGLPipeline(GLuint vertexShader_, GLuint fragmentShader_);
    ~OpenGLPipeline() override;

   public:
    void setProperty(std::string property, const void* value) override;
    /*void setDiffuse(OpenGLTexture*);*/

    // void setProperties(std::vector<MaterialProperty> inProperties) override;
    void setProperties(const MaterialStructure& inStructure) override;

   private:
    void bind() override;
    void unbind() override;

    // ShaderPtr shader_;
    // ShaderPtr& getShader();

    GLuint shaderProgram_{0};

    [[nodiscard]] static std::vector<PipelineAttribute> getAttributesFromShader(
        GLuint program);
    [[nodiscard]] static std::vector<PipelineUniform> getuniformsFromShader(
        GLuint program);
};
}  // namespace Presto
