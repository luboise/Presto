#include "OpenGLPipeline.h"
#include <GL/glext.h>
#include <algorithm>
#include <numeric>

#include "Presto/Rendering/UniformTypes.h"
#include "Rendering/OpenGL/utils.h"
#include "Rendering/Utils/RenderingUtils.h"

#include "Presto/Rendering/UniformBuffer.h"

namespace Presto {
using Presto::UniformVariableType;

OpenGLPipeline::OpenGLPipeline(GLuint vertexShader_, GLuint fragmentShader_)
    : shaderProgram_(glCreateProgram()) {
    glAttachShader(shaderProgram_, vertexShader_);
    glAttachShader(shaderProgram_, fragmentShader_);

    glLinkProgram(shaderProgram_);
    PR_ASSERT(OpenGLUtils::ShaderProgramLinkedCorrectly(shaderProgram_),
              "Shader program failed to link.");

    pipelineStructure_.attributes = getAttributesFromShader(shaderProgram_);
    pipelineStructure_.uniforms = getUniformsFromShader(shaderProgram_);
    pipelineStructure_.uniform_blocks =
        getUniformBlocksFromShader(shaderProgram_);

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

void OpenGLPipeline::bind() { glUseProgram(shaderProgram_); }

void OpenGLPipeline::unbind() { glUseProgram(0); }

void OpenGLPipeline::setProperty(std::string /*property*/,
                                 const void* /*value*/) {};

// ShaderPtr& OpenGLPipeline::getShader() { return shader_; };

/*void OpenGLPipeline::setDiffuse(OpenGLTexture* image) {*/
/*    diffuseTexture_ = image;*/
/*};*/

void OpenGLPipeline::setProperties(const MaterialStructure& inStructure) {
    Presto::size_t uniform_byte_sum{
        std::accumulate(pipelineStructure_.uniforms.begin(),
                        pipelineStructure_.uniforms.end(), Presto::size_t{0},
                        [](Presto::size_t sum, const PipelineUniform& uniform) {
                            return sum + SizeOfType(uniform.type);
                        })};

    std::vector<std::byte> bytes_to_write(uniform_byte_sum, std::byte{0});

    for (const auto& uniform : pipelineStructure_.uniforms) {
        const auto& name{uniform.name};
        const auto* in_property{inStructure.getProperty(name)};

        if (in_property == nullptr) {
            PR_CORE_WARN(std::format(
                "Missing uniform value \"{}\" required by pipeline {}. "
                "Skipping this property.",
                name, this->id()));
            continue;
        }

        SizeOfType(uniform.type);
        auto bytes{in_property->data.bytes()};

        // TODO: Put overflow checks here
        std::memcpy(&bytes_to_write[uniform.offset], bytes.data(),
                    bytes.size());
    }
}

OpenGLPipeline::~OpenGLPipeline() { glDeleteProgram(shaderProgram_); };
;

void OpenGLPipeline::setUniformBlock(PR_NUMERIC_ID name,
                                     UniformBuffer& buffer) {
    Presto::size_t index{};
    buffer.bind(index);
};
}  // namespace Presto
