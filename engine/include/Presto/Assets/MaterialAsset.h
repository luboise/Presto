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

    MaterialAsset(PR_STRING_ID name, const PipelineStructure&);

    ~MaterialAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MATERIAL_DEFINITION;
    };

    // [[nodiscard]] MaterialInstancePtr createInstance() const;

    [[nodiscard]] MaterialStructure getStructure() const;

    // Checks if a material asset is equal to a specific pipeline
    /*bool operator==(renderer_pipeline_id_t id) const;*/

    static MaterialStructure materialStructureFromPipeline(
        const PipelineStructure&);

    Ptr<MaterialInstance> createInstance();

    [[nodiscard]] pipeline_id_t pipelineId() const;

   private:
    void load() override;

    pipeline_id_t pipelineId_{PR_PIPELINE_DEFAULT_3D};

    MaterialStructure structure_;

    // PipelineStructure structure_{};

    std::vector<MaterialInstance> instances_;
    std::vector<MaterialProperty> properties_;
};

using MaterialDefinitionPtr = AssetPtr<MaterialAsset>;

}  // namespace Presto
