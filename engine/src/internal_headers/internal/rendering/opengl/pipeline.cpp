#include "./pipeline.h"
#include "internal/rendering/opengl/opengl.h"

#include <GL/glext.h>
#include <algorithm>
#include "OpenGLPipeline.h"

#include "Presto/Rendering/UniformTypes.h"
#include "Rendering/OpenGL/utils.h"
#include "presto/assets/importing.h"

#include "Presto/Rendering/UniformBuffer.h"

#include "Introspection.h"

namespace Pr {
using Pr::UniformVariableType;

OpenGLPipeline::OpenGLPipeline(
    pipeline_id_t id, GLuint vertexShader, GLuint fragmentShader,
    const std::vector<PipelineAttribute>& attributesOverride)
    : Pipeline(id), shaderProgram_(glCreateProgram()) {
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);

    glLinkProgram(shaderProgram_);
    PR_ASSERT(OpenGLUtils::ShaderProgramLinkedCorrectly(shaderProgram_),
              "Shader program failed to link.");

    if (!attributesOverride.empty()) {
        pipelineStructure_.attributes = attributesOverride;
    } else {
        pipelineStructure_.attributes =
            Introspection::getAttributesFromShader(shaderProgram_);
    }

    auto color_attrib{std::ranges::find_if(
        pipelineStructure_.attributes, [](PipelineAttribute& attrib) {
            return attrib.name == DefaultAttributeName::COLOUR;
        })};
    if (color_attrib == pipelineStructure_.attributes.end()) {
        pipelineStructure_.uses_alpha_channel = false;
    } else {
        pipelineStructure_.uses_alpha_channel =
            color_attrib->type == ShaderDataType::VEC4;
    }

    pipelineStructure_.uniforms =
        Introspection::getUniformsFromShader(shaderProgram_);
    pipelineStructure_.uniform_blocks =
        Introspection::getUniformBlocksFromShader(shaderProgram_);

    // Find all bound uniform textures
    GLint location{};
    for (PipelineUniform& uniform : pipelineStructure_.uniforms) {
        if (uniform.data_type != UniformVariableType::TEXTURE) {
            continue;
        }

        location = glGetUniformLocation(shaderProgram_, uniform.name.data());
        GLint bind_point{};
        glGetUniformiv(shaderProgram_, location, &bind_point);

        uniform.location = static_cast<decltype(uniform.location)>(bind_point);
    }

    pipelineStructure_.uses_global_uniforms =
        std::ranges::any_of(pipelineStructure_.uniform_blocks,
                            [](const PipelineUniformBlock& block) -> bool {
                                return block.name == "GlobalUniforms";
                            });

    pipelineStructure_.uses_object_uniforms =
        std::ranges::any_of(pipelineStructure_.uniform_blocks,
                            [](const PipelineUniformBlock& block) -> bool {
                                return block.name == "ObjectUniforms";
                            });
};

void OpenGLPipeline::bind() {
    if (this->pipelineStructure_.uses_alpha_channel) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    glUseProgram(shaderProgram_);
}

void OpenGLPipeline::unbind() { glUseProgram(0); }

OpenGLPipeline::~OpenGLPipeline() { glDeleteProgram(shaderProgram_); };

uniform_index_t OpenGLPipeline::getIndex(uniform_name_t name) {
    if (auto found{nameToIndex_.find(name)}; found != nameToIndex_.end()) {
        return found->second;
    }

    return PR_INVALID_UNIFORM;
};

void OpenGLPipeline::setUniformBlock(uniform_index_t index,
                                     UniformBuffer& buffer) {
    buffer.bind(index);
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::mat4 value) {
    glUniformMatrix4fv(indexToBinding_[index], 1, GL_FALSE,
                       glm::value_ptr(value));
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::float32_t value) {
    glUniform1f(indexToBinding_[index], value);
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::int32_t value) {
    glUniform1i(indexToBinding_[index], value);
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::uint32_t value) {
    glUniform1ui(indexToBinding_[index], value);
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::vec2 value) {
    glUniform2fv(indexToBinding_[index], 1, glm::value_ptr(value));
};

void OpenGLPipeline::setUniform(uniform_index_t index, Pr::vec3 value) {
    glUniform3fv(indexToBinding_[index], 1, glm::value_ptr(value));
};
void OpenGLPipeline::setUniform(uniform_index_t index, Pr::vec4 value) {
    glUniform4fv(indexToBinding_[index], 1, glm::value_ptr(value));
};

}  // namespace Pr
