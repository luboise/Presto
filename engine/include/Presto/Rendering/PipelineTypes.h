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
};

struct PipelineUniform {
    PR_NUMERIC_ID binding;
    UniformVariableType type;
    PR_STRING_ID name;
};

struct PipelineStructure {
    renderer_pipeline_id_t pipeline_id;
    std::vector<PipelineAttribute> attributes;
};

}  // namespace Presto
