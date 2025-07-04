module;
#include "presto/platform.h"

export module presto.assets.material;

import std;

import presto.assets.types;
import presto.core.types;

import presto.assets.material.types;

// Private fwd declarations
namespace Pr {
struct PipelineStructure;
}  // namespace Pr

export namespace Pr {

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

// using Pr::Ptr<Pr::MaterialAsset> = AssetPtr<MaterialAsset>;

}  // namespace Pr
