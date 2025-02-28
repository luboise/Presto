#pragma once

#include <type_traits>
#include "Presto/Aliases/Handles.h"

#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/UniformBuffer.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Types/AssetTypes.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

class ErasedBytes;
struct UniformLayout;

struct UniformBinding;

class MaterialInstance {
    friend class MaterialAsset;
    friend class RenderingManager;

   public:
    explicit MaterialInstance(const MaterialDefinitionPtr&);
    ~MaterialInstance();

    MaterialInstance& setName(Presto::string newName);
    [[nodiscard]] Presto::string name() const;

    template <typename T>
        requires requires { sizeof(T); }
    MaterialInstance& setProperty(Presto::string name, const T& data) {
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
                "The size of data being written to material property {} must "
                "be of "
                "size {}. Received size {}.",
                name, details->binding.size(), sizeof(data));
            return *this;
        }

        switch (details->binding.bind_type) {
            case UniformBinding::SINGLE: {
                getUniformDataStore(details->data_index).write(data);
                break;
            }
            case UniformBinding::BLOCK: {
                ErasedBytes bytes{data};

                getUniformBuffer(details->data_index)
                    .write(bytes.getData(), details->binding.offset);
                break;
            }
            default: {
                PR_ERROR("Unhandled UniformBinding case.");
            }
        }

        return *this;
    };

    /*
MaterialInstance& setProperty(Presto::string name, Presto::vec3 data);
MaterialInstance& setProperty(Presto::string name, ErasedBytes data);
MaterialInstance& setProperty(Presto::string name, TexturePtr data);
    */

    [[nodiscard]] const UniformLayout& getUniformStructure() const;

    [[nodiscard]] pipeline_id_t getPipelineId() const;

   private:
    void bind();

    struct PropertyDetails {
        UniformBinding binding;
        // Data index is the index of the uniform block for block variables, and
        // the index of the data for regular uniform variables
        Presto::size_t data_index{};
    };

    PropertyDetails* getBinding(const Presto::string& name);
    UniformBuffer& getUniformBuffer(Presto::size_t index);
    ErasedBytes& getUniformDataStore(Presto::size_t index);

    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
