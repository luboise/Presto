#include "presto/assets/material.h"

#include "internal/managers/rendering_manager.h"

#include <utility>

namespace Pr {
MaterialAsset::MaterialAsset(Pr::string name, const PipelineStructure& ps)
    : Asset(std::move(name)), pipelineId_(ps.pipeline_id) {
    UniformLayout layout{createLayoutFromPipelineStructure(ps)};

    // TODO: Put validation checks here before overwriting
    this->uniformLayout_ = layout;
}

UniformLayout MaterialAsset::uniformLayout() const { return uniformLayout_; }

bool MaterialAsset::load() { return true; };

UniformLayout MaterialAsset::createLayoutFromPipelineStructure(
    const PipelineStructure& ps) {
    UniformLayout layout{};

    for (const PipelineUniformBlock& block : ps.uniform_blocks) {
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

    layout.bindings.resize(ps.uniforms.size());

    for (Pr::size_t i{0}; i < ps.uniforms.size(); i++) {
        const PipelineUniform& uniform{ps.uniforms[i]};

        layout.bindings[i] = {
            .bind_type = UniformBinding::SINGLE,
            .data_type = uniform.data_type,
            .name = uniform.name,
            .location = uniform.location,
        };
    }

    return layout;
};

Ptr<MaterialInstance> MaterialAsset::createInstance() {
    return std::make_shared<MaterialInstance>(shared_from_this());
};

}  // namespace Pr
