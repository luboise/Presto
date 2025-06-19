module presto.internal.rendering:pipeline;

import presto.core;

#include <numeric>

namespace Presto {

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

Presto::size_t PipelineUniformBlock::size() const {
    return std::accumulate(
        this->uniforms.begin(), this->uniforms.end(), Presto::size_t{0},
        [](auto sum, const auto& uniform) { return sum + uniform.size(); });
};

Presto::size_t PipelineUniform::size() const {
    return SizeOfType(this->data_type);
};

Presto::size_t PipelineAttribute::size() const {
    return SizeOfShaderType(this->type);
};

Presto::size_t PipelineStructure::stride() const {
    return std::accumulate(this->attributes.begin(), this->attributes.end(),
                           Presto::size_t{0},
                           [](auto sum, const PipelineAttribute& attrib) {
                               return sum + attrib.size();
                           });
};

}  // namespace Presto
