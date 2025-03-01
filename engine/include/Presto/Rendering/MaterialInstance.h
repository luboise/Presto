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

class Pipeline;

template <typename, typename = void>
struct has_arrow_operator : std::false_type {};

template <typename T>
struct has_arrow_operator<T,
                          std::void_t<decltype(std::declval<T>().operator->())>>
    : std::true_type {};

template <typename T>
inline constexpr bool has_arrow_operator_v = has_arrow_operator<T>::value;

template <typename T>
inline constexpr bool is_any_pointer_type_v =
    (has_arrow_operator_v<T> || std::is_pointer_v<T>);

class MaterialInstance {
    friend class MaterialAsset;
    friend class RenderingManager;

   public:
    explicit MaterialInstance(const MaterialDefinitionPtr&);
    ~MaterialInstance();

    MaterialInstance& setName(Presto::string newName);
    [[nodiscard]] Presto::string name() const;

    [[nodiscard]] const UniformLayout& getUniformStructure() const;

    [[nodiscard]] pipeline_id_t getPipelineId() const;

    template <typename T>
        requires requires { sizeof(T); } &&
                 // Ensure that T is not a pointer
                 requires { !is_any_pointer_type_v<T>; }
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

   private:
    void bindTo(Pipeline&) const;

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

template <>
MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                const Ptr<Texture>& data);

}  // namespace Presto
