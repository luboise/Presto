#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"

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

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MATERIAL;
    };

    [[nodiscard]] MaterialStructure getStructure() const;

    void setDiffuseTexture(ImagePtr image);

    [[nodiscard]] ImagePtr getImage() const;

   private:
    void load() override;

    template <typename T>
    void setProperty(PR_STRING_ID name, T value) {
        auto* property{getProperty(name)};
        if (property == nullptr) {
            PR_CORE_ERROR(
                std::format("Property not found in material: {}", name));
            return;
        }

        property->data.setData(value);
    };

    MaterialProperty* getProperty(const PR_STRING_ID& name);

    renderer_pipeline_id_t pipelineId_{PR_PIPELINE_DEFAULT_3D};

    std::vector<MaterialProperty> properties_;

    ImagePtr diffuseImage_;
};

using MaterialPtr = AssetPtr<MaterialAsset>;
}  // namespace Presto
