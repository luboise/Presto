module;
#include "presto/platform.h"

export module presto.assets.material;

import presto.core.types;
import presto.core.constants;
import presto.core.concepts;
export import presto.assets.material.types;
export import presto.assets.material.traits;

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

// using Pr::Ptr<Pr::MaterialAsset> = AssetPtr<MaterialAsset>;

}  // namespace Pr
