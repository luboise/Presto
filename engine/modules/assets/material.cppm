module;
#include "presto/platform.h"

export module presto.assets.material;

export import presto.assets.material.types;
export import presto.assets.material.traits;

import std;

import presto.assets.types;

export namespace Pr {

class MaterialInstance;

class PRESTO_API MaterialAsset final :
    // Enables MaterialAsset to be able to get its own shared pointer, needed
    // for giving the instances a weak reference
    public std::enable_shared_from_this<MaterialAsset>,
    public Asset {
   public:
    friend class RenderingManager;

    MaterialAsset(Pr::string name, const PipelineStructure& structure);

    ~MaterialAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MATERIAL_DEFINITION;
    };

    /**
     * @brief  Retrieves the uniform layout of the MaterialAsset. This is
     * currently only used when creating a new MaterialInstance.
     */
    [[nodiscard]] UniformLayout uniformLayout() const;

    static UniformLayout createLayoutFromPipelineStructure(
        const PipelineStructure&);

    Ptr<MaterialInstance> createInstance();

    [[nodiscard]] pipeline_id_t pipelineId() const { return pipelineId_; };

   private:
    bool load() override;

    pipeline_id_t pipelineId_{PR_PIPELINE_NONE};
    UniformLayout uniformLayout_;

    // PipelineStructure structure_{};

    std::vector<MaterialInstance> instances_;
    // std::vector<MaterialProperty> properties_;
};

class MaterialInstance {
    friend class MaterialAsset;
    friend class RenderingManager;

   public:
    explicit MaterialInstance(const Pr::Ptr<Pr::MaterialAsset>&);
    ~MaterialInstance();

    MaterialInstance& setName(Pr::string newName);
    [[nodiscard]] Pr::string name() const;

    [[nodiscard]] const UniformLayout& getUniformStructure() const;

    [[nodiscard]] pipeline_id_t getPipelineId() const;

    void setFromImport(const ImportedMaterial& imported_material,
                       std::vector<Pr::Ptr<Pr::Texture>>& texturePtrs);

    template <typename T>
        requires requires { sizeof(T); } &&
                 // Ensure that T is not a pointer
                 requires { !is_any_pointer_type_v<T>; }
    MaterialInstance& setProperty(Pr::string name, const T& data) {
        PropertyDetails* details{getBinding(name)};

        if (details == nullptr) {
            Pr::CoreLog(
                WARN,
                "Unable to find \"{}\" in MaterialInstance of pipeline {}. "
                "Skipping this write.",
                name, this->getPipelineId());
            return *this;
        }

        if (sizeof(data) != details->binding.size()) {
            Pr::CoreLog(
                ERROR,
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
                Pr::CoreLog(ERROR, "Unhandled UniformBinding case.");
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
        Pr::size_t data_index{};
    };

    PropertyDetails* getBinding(const Pr::string& name);
    UniformBuffer& getUniformBuffer(Pr::size_t index);
    ErasedBytes& getUniformDataStore(Pr::size_t index);

    struct Impl;
    Allocated<Impl> impl_;
};

template <>
MaterialInstance& MaterialInstance::setProperty(Pr::string name,
                                                const Ptr<Texture>& data);

// using Pr::Ptr<Pr::MaterialAsset> = AssetPtr<MaterialAsset>;

}  // namespace Pr
