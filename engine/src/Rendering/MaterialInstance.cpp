#include <map>

#include <utility>

#include "Modules/RenderingManager.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/MaterialTypes.h"

#include "Presto/Rendering/UniformBuffer.h"
#include "Presto/Rendering/UniformTypes.h"

#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

struct MaterialInstance::Impl {
    MaterialDefinitionPtr definition;
    Presto::string name;

    UniformLayout structure;

    std::map<Presto::string, PropertyDetails> property_lookup;

    std::vector<Allocated<UniformBuffer>> uniform_buffers;
    std::vector<ErasedBytes> uniform_variable_data_stores;
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

        impl_->uniform_buffers[i] = rm.createUniformBuffer(block.size());

        for (const UniformBinding& binding : block.bindings) {
            impl_->property_lookup[binding.name] = {
                .binding{binding},
                .data_index = i,
            };
        }
    }

    impl_->uniform_variable_data_stores.resize(
        impl_->structure.bindings.size());
    for (Presto::size_t i = 0; i < impl_->structure.bindings.size(); i++) {
        const UniformBinding& binding{impl_->structure.bindings[i]};
        impl_->property_lookup[binding.name] = {.binding = binding,
                                                .data_index = i};

        impl_->uniform_variable_data_stores[i] =
            ErasedBytes{ByteArray(binding.size())};
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

void MaterialInstance::bind() {
    // Bind each block
    for (const auto& buffer : impl_->property_lookup) {
        static_assert(false, "Implement this");
    }
    // Bind each singular uniform
};

Presto::string MaterialInstance::name() const { return impl_->name; };

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
