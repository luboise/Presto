#pragma once

#include <GL/glew.h>
#include <map>

#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

class OpenGLPipeline final : public Pipeline {
    friend class OpenGLRenderer;
    friend class OpenGLDrawManager;
    friend class OpenGLPipelineBuilder;

    explicit OpenGLPipeline(GLuint vertexShader_, GLuint fragmentShader_);
    ~OpenGLPipeline() override;

   public:
    uniform_index_t getIndex(uniform_name_t name) override;

    void setUniform(uniform_index_t index, Presto::uint32_t value) override;

    void setUniform(uniform_index_t index, Presto::int32_t value) override;
    void setUniform(uniform_index_t index, Presto::float32_t value) override;

    void setUniform(uniform_index_t index, Presto::vec2 value) override;
    void setUniform(uniform_index_t index, Presto::vec3 value) override;
    void setUniform(uniform_index_t index, Presto::vec4 value) override;
    void setUniform(uniform_index_t index, Presto::mat4 value) override;

    // void setProperties(const UniformLayout& inStructure) override;

    void setUniformBlock(uniform_index_t index, UniformBuffer& buffer) override;

   private:
    void bind() override;
    void unbind() override;

    // Map which converts the name of a uniform to its external index
    std::map<uniform_name_t, uniform_index_t> nameToIndex_;

    // Map which converts the name of a uniform to its external index
    std::map<uniform_index_t, GLint> indexToBinding_;

    GLuint shaderProgram_{0};

    [[nodiscard]] static std::vector<PipelineAttribute> getAttributesFromShader(
        GLuint program);
    [[nodiscard]] static std::vector<PipelineUniform> getUniformsFromShader(
        GLuint program);
    [[nodiscard]] static std::vector<PipelineUniformBlock>
    getUniformBlocksFromShader(GLuint program);
};
}  // namespace Presto
