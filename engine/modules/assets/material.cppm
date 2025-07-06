module;
#include "presto/platform.h"

export module presto.assets.material;

export import presto.types.material;
export import presto.assets.material.traits;

import presto.types;
import presto.core.constants;
import presto.core.concepts;

import std;

import presto.assets.types;

export namespace Pr {

class PRESTO_API MaterialAsset final :
    // Enables MaterialAsset to be able to get its own shared pointer, needed
    // for giving the instances a weak reference
    public std::enable_shared_from_this<MaterialAsset>,
    public Asset {
   public:
    friend class RenderingManager;

    MaterialAsset(Pr::string name, const UniformLayout& structure);

    ~MaterialAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MATERIAL_DEFINITION;
    };

    /**
     * @brief  Retrieves the uniform layout of the MaterialAsset. This is
     * currently only used when creating a new MaterialInstance.
     */
    [[nodiscard]] UniformLayout uniformLayout() const;

    // Ptr<MaterialInstance> createInstance();

    [[nodiscard]] pipeline_id_t pipelineId() const { return pipelineId_; };

   private:
    bool load() override;

    pipeline_id_t pipelineId_{PR_PIPELINE_NONE};
    UniformLayout uniformLayout_;
};

class MaterialInstance {
    friend class MaterialAsset;

   public:
    virtual MaterialInstance& setName(Pr::string newName) = 0;
    [[nodiscard]] virtual Pr::string name() const = 0;

    [[nodiscard]] virtual const UniformLayout& getUniformStructure() const = 0;
    [[nodiscard]] virtual pipeline_id_t getPipelineId() const = 0;
};

// using Pr::Ptr<Pr::MaterialAsset> = AssetPtr<MaterialAsset>;

}  // namespace Pr
