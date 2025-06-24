#include "internal/rendering.h"
import :pipeline;

#include "presto/core.h"

#include <numeric>

namespace Pr {

pipeline_id_t Pipeline::id() const { return pipelineStructure_.pipeline_id; }

void Pipeline::setId(pipeline_id_t id) {
    this->pipelineStructure_.pipeline_id = id;
}

Pipeline::Pipeline(pipeline_id_t id) { setId(id); }

PipelineBuilder& PipelineBuilder::setShader(const AssetArg& shaderPath,
                                            ShaderStage type) {
    auto data = Utils::File::ReadFile(shaderPath);
    return setShader(data.data(), type);
};

Pr::size_t PipelineUniformBlock::size() const {
    return std::accumulate(
        this->uniforms.begin(), this->uniforms.end(), Pr::size_t{0},
        [](auto sum, const auto& uniform) { return sum + uniform.size(); });
};

Pr::size_t PipelineUniform::size() const {
    return SizeOfType(this->data_type);
};

Pr::size_t PipelineAttribute::size() const {
    return SizeOfShaderType(this->type);
};

Pr::size_t PipelineStructure::stride() const {
    return std::accumulate(this->attributes.begin(), this->attributes.end(),
                           Pr::size_t{0},
                           [](auto sum, const PipelineAttribute& attrib) {
                               return sum + attrib.size();
                           });
};

}  // namespace Pr
