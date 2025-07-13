module presto.internal.rendering;
import :pipeline;

import presto.core;
import presto.types;
import presto.utils;

import presto.assets.asset;

import std;

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

UniformLayout PipelineStructure::asUniformLayout() const {
    UniformLayout layout{};

    for (const PipelineUniformBlock& block : this->uniform_blocks) {
        // Ignore the basic uniforms when creating a material layout
        if (block.name == "GlobalUniforms" || block.name == "ObjectUniforms") {
            continue;
        }

        UniformBlock new_block{};

        new_block.bind_point = block.binding;
        new_block.name = block.name;

        Pr::uint32_t running_offset{0};

        for (const PipelineUniform& uniform : block.uniforms) {
            new_block.bindings.push_back({
                .bind_type = UniformBinding::BLOCK,
                .data_type = uniform.data_type,
                .name = uniform.name,
                .offset = running_offset,
            });
            running_offset += uniform.size();
        }

        layout.blocks.push_back(new_block);
    }

    layout.bindings.resize(this->uniforms.size());

    for (Pr::size_t i{0}; i < this->uniforms.size(); i++) {
        const PipelineUniform& uniform{this->uniforms[i]};

        layout.bindings[i] = {
            .bind_type = UniformBinding::SINGLE,
            .data_type = uniform.data_type,
            .name = uniform.name,
            .location = uniform.location,
        };
    }

    return layout;
};

}  // namespace Pr
