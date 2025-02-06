#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"

#include "Presto/Rendering/MaterialData.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
class MaterialAsset final : public Asset {
   public:
    friend class RenderingManager;

    explicit MaterialAsset(PR_STRING_ID name,
                           const ImportedMaterial& importedMaterial);
    ~MaterialAsset() override = default;

    [[nodiscard]] constexpr AssetType getType() const override {
        return AssetType::MATERIAL;
    };

    [[nodiscard]] MaterialStructure getStructure() const;

    void setDiffuseTexture(ImagePtr image);

    [[nodiscard]] ImagePtr getImage() const;

   private:
    void load() override;

    renderer_pipeline_id_t pipelineId_{PR_PIPELINE_DEFAULT_3D};

    std::vector<MaterialProperty> properties_;

    ImagePtr diffuseImage_;
};

using MaterialPtr = AssetPtr<MaterialAsset>;
}  // namespace Presto
