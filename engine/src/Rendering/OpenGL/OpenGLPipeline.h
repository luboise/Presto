#pragma once

#include <GL/glew.h>
#include <map>

#include "Presto/Rendering/Pipeline.h"

namespace Presto {

class OpenGLPipeline final : public Pipeline {
    friend class OpenGLRenderer;
    friend class OpenGLDrawManager;
    friend class OpenGLPipelineBuilder;

    explicit OpenGLPipeline(GLuint vertexShader_, GLuint fragmentShader_);
    ~OpenGLPipeline() override;

   public:
    void setProperty(std::string property, const void* value) override;
    // void setProperties(const UniformLayout& inStructure) override;

    void setUniformBlock(PR_NUMERIC_ID name, UniformBuffer& buffer) override;

   private:
    void bind() override;
    void unbind() override;

    std::map<PR_NUMERIC_ID, GLint> bindingMap_;

    GLuint shaderProgram_{0};

    [[nodiscard]] static std::vector<PipelineAttribute> getAttributesFromShader(
        GLuint program);
    [[nodiscard]] static std::vector<PipelineUniform> getUniformsFromShader(
        GLuint program);
    [[nodiscard]] static std::vector<PipelineUniformBlock>
    getUniformBlocksFromShader(GLuint program);
};
}  // namespace Presto
