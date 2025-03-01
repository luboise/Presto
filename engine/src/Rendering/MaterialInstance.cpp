#include <map>

#include <utility>

#include "Modules/RenderingManager.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/MaterialTypes.h"

#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/UniformBuffer.h"
#include "Presto/Rendering/UniformTypes.h"

#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

struct UniformBufferExtra {
    Presto::uint8_t bind_point;
    Allocated<UniformBuffer> buffer;
};

struct UniformBindingExtra {
    Presto::uint8_t location;

    UniformVariableType data_type;
    ErasedBytes data;
};

struct MaterialInstance::Impl {
    MaterialDefinitionPtr definition;
    Presto::string name;

    UniformLayout structure;

    std::map<Presto::string, PropertyDetails> property_lookup;

    std::vector<UniformBufferExtra> uniform_buffers;
    std::vector<UniformBindingExtra> uniform_bindings;
    std::vector<TexturePtr> textures;
};

MaterialInstance::MaterialInstance(const MaterialDefinitionPtr& definition) {
    impl_ = std::make_unique<Impl>();
    impl_->definition = definition;
    impl_->structure = definition->uniformLayout();

    RenderingManager& rm{RenderingManager::get()};

    // Allocate a buffer for each uniform block (needed for hotswap)
    impl_->uniform_buffers.resize(impl_->structure.blocks.size());
    for (Presto::size_t i = 0; i < impl_->structure.blocks.size(); i++) {
        const UniformBlock& block{impl_->structure.blocks[i]};

        impl_->uniform_buffers[i] = {
            .bind_point = block.bind_point,
            .buffer = rm.createUniformBuffer(block.size())};

        for (const UniformBinding& binding : block.bindings) {
            impl_->property_lookup[binding.name] = {
                .binding{binding},
                .data_index = i,
            };
        }
    }

    Presto::size_t naked_binding_count{impl_->structure.bindings.size()};

    impl_->uniform_bindings.resize(naked_binding_count);
    for (Presto::size_t i = 0; i < naked_binding_count; i++) {
        const UniformBinding& binding{impl_->structure.bindings[i]};
        impl_->property_lookup[binding.name] = {.binding = binding,
                                                .data_index = i};

        impl_->uniform_bindings[i] = {
            .location = static_cast<Presto::uint8_t>(binding.location),
            .data_type = binding.data_type,
            .data = ErasedBytes{ByteArray(binding.size())}};

        if (binding.data_type == UniformVariableType::TEXTURE) {
            impl_->uniform_bindings[i].data.write(
                static_cast<ImportTypeOf<UniformVariableType::TEXTURE>>(
                    impl_->textures.size()));
            impl_->textures.push_back(nullptr);
        }
    }
}

MaterialInstance::~MaterialInstance() = default;

MaterialInstance::PropertyDetails* MaterialInstance::getBinding(
    const Presto::string& name) {
    if (auto val{impl_->property_lookup.find(name)};
        val != impl_->property_lookup.end()) {
        return &(val->second);
    }
    return nullptr;
};

MaterialInstance& MaterialInstance::setName(Presto::string newName) {
    impl_->name = std::move(newName);
    return *this;
};

ErasedBytes& MaterialInstance::getUniformDataStore(Presto::size_t index) {
    return impl_->uniform_bindings[index].data;
};

UniformBuffer& MaterialInstance::getUniformBuffer(Presto::size_t index) {
    return *impl_->uniform_buffers[index].buffer;
};

const UniformLayout& MaterialInstance::getUniformStructure() const {
    return this->impl_->structure;
};

pipeline_id_t MaterialInstance::getPipelineId() const {
    return this->impl_->definition->pipelineId();
};

Presto::string MaterialInstance::name() const { return impl_->name; };

void MaterialInstance::bindTo(Pipeline& pipeline) const {
    // Bind each block
    for (const UniformBufferExtra& buffer_details : impl_->uniform_buffers) {
        pipeline.setUniformBlock(buffer_details.bind_point,
                                 *buffer_details.buffer);
    }
    // Bind each singular uniform

    for (const UniformBindingExtra& binding : impl_->uniform_bindings) {
#define SWITCH_CASE(type)                                          \
    case type:                                                     \
        pipeline.setUniform(binding.location,                      \
                            binding.data.as<GPUTypeOf<(type)>>()); \
        break

        switch (binding.data_type) {
            SWITCH_CASE(UniformVariableType::INT);
            SWITCH_CASE(UniformVariableType::UINT);
            SWITCH_CASE(UniformVariableType::FLOAT);
            SWITCH_CASE(UniformVariableType::VEC2);
            SWITCH_CASE(UniformVariableType::VEC3);
            SWITCH_CASE(UniformVariableType::VEC4);
            SWITCH_CASE(UniformVariableType::MAT4);

            case UniformVariableType::TEXTURE: {
                const TexturePtr& texture{
                    impl_->textures[binding.data.as<Presto::uint8_t>()]};
                if (texture == nullptr) {
                    PR_TRACE(
                        "No texture specified at location {} in material "
                        "instance. Skipping this write.",
                        binding.location);
                    continue;
                }
                texture->bind(binding.location);

                break;
            }

            default:
                PR_ERROR(
                    "Unable to determine type of binding data. Skipping "
                    "this "
                    "write.");
                break;
        }
#undef SWITCH_CASE
    }
};

template <>
MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                const Ptr<Texture>& data) {
    PropertyDetails* details{getBinding(name)};

    if (details == nullptr) {
        PR_WARN(
            "Unable to find texture \"{}\" in MaterialInstance of pipeline "
            "{}. "
            "Skipping this write.",
            name, this->getPipelineId());
        return *this;
    }

    if (details->binding.data_type != UniformVariableType::TEXTURE) {
        PR_ERROR(
            "Unable to write Texture value to \"{}\" in pipeline {}. (It "
            "is "
            "not a texture type). Skipping this write.",
            name, this->getPipelineId());
        return *this;
    }

    auto texture_index{getUniformDataStore(details->data_index)
                           .as<ImportTypeOf<UniformVariableType::TEXTURE>>()};
    PR_CORE_ASSERT(
        texture_index < impl_->textures.size(),
        std::format("Texture index {} must be in bounds for array of size {}.",
                    texture_index, impl_->textures.size()));

    impl_->textures[texture_index] = data;

    return *this;
};

}  // namespace Presto
