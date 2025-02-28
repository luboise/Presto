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
    }
}

MaterialInstance::~MaterialInstance() = default;

MaterialInstance::PropertyDetails* MaterialInstance::getBinding(
    const Presto::string& name) {
    if (auto val{impl_->property_lookup.find(name)};
        val == impl_->property_lookup.end()) {
        return &(val->second);
    }
    return nullptr;
};

MaterialInstance& MaterialInstance::setName(Presto::string newName) {
    impl_->name = std::move(newName);
    return *this;
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
            SWITCH_CASE(UniformVariableType::TEXTURE);
            default:
                PR_ERROR(
                    "Unable to determine type of binding data. Skipping this "
                    "write.");
                break;
        }
#undef SWITCH_CASE
    }
};

/*
MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Presto::vec3 data) {
    PropertyDetails* details{getBinding(name)};

    if (details == nullptr) {
        PR_WARN(
            "Unable to find \"{}\" in MaterialInstance of pipeline {}. "
            "Skipping this write.",
            name, this->getPipelineId());
        return *this;
    }

    if (sizeof(data) != details->binding.size()) {
        PR_ERROR(
            "The size of data being written to material property {} must be of "
            "size {}. Received size {}.",
            name, details->binding.size(), sizeof(data));
        return *this;
    }

    if (details->binding.bind_type == UniformBinding::SINGLE) {
        impl_->uniform_variable_data_stores[details->data_index].write(data);
    }
    return *this;
};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                ErasedBytes data) {
    return *this;
};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Ptr<Texture> data) {
    return *this;
};
*/
}  // namespace Presto
