#pragma once

#include "Presto/Rendering/AttributeTypes.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

using renderer_pipeline_id_t = PR_NUMERIC_ID;

struct PipelineAttribute {
    // Index of the attribute
    PR_NUMERIC_ID layout;

    ShaderDataType type;
    PR_STRING_ID name;

    Presto::size_t offset;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineUniform {
    PR_NUMERIC_ID location;
    UniformVariableType type;
    PR_STRING_ID name;

    Presto::size_t offset;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineUniformBlock {
    PR_NUMERIC_ID binding;
    PR_STRING_ID name;
    std::vector<PipelineUniform> uniforms;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineStructure {
    pipeline_id_t pipeline_id;

    bool uses_global_uniforms;
    bool uses_object_uniforms;

    std::vector<PipelineAttribute> attributes;
    std::vector<PipelineUniform> uniforms;
    std::vector<PipelineUniformBlock> uniform_blocks;

    [[nodiscard]] Presto::size_t stride() const;
};

}  // namespace Presto
