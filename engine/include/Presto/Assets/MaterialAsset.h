#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
class MaterialInstanceAsset;
using MaterialInstancePtr = Ptr<MaterialInstanceAsset>;

class MaterialAsset final : public Asset {
   public:
    friend class RenderingManager;

    explicit MaterialAsset(PR_STRING_ID name,
                           const ImportedMaterial& importedMaterial);
    ~MaterialAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MATERIAL;
    };

    [[nodiscard]] MaterialStructure getStructure() const;

    [[nodiscard]] ImagePtr getImage() const;

    // Checks if a material asset is equal to a specific pipeline
    bool operator==(renderer_pipeline_id_t id) const;

    [[nodiscard]] MaterialInstancePtr createInstance() const;

   private:
    void load() override;

    renderer_pipeline_id_t pipelineId_{PR_PIPELINE_DEFAULT_3D};

    std::vector<MaterialProperty> properties_;

    ImagePtr diffuseImage_;
};

using MaterialPtr = AssetPtr<MaterialAsset>;
}  // namespace Presto
