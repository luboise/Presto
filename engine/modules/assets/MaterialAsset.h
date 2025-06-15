#pragma once

#include <memory>

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Rendering/MaterialTypes.h"

#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

class MaterialAsset final :
    // Enables MaterialAsset to be able to get its own shared pointer, needed
    // for giving the instances a weak reference
    public std::enable_shared_from_this<MaterialAsset>,
    public Asset {
   public:
    friend class RenderingManager;

    MaterialAsset(Presto::string name, const PipelineStructure& structure);

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

using MaterialDefinitionPtr = AssetPtr<MaterialAsset>;

}  // namespace Presto
