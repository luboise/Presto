#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/MaterialData.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/Image.h"
#include "Presto/Assets/ImageAsset.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    class MaterialAsset final : public Asset {
       public:
        friend class RenderingManager;

        explicit MaterialAsset(PR_STRING_ID name, ImagePtr image = nullptr);
        ~MaterialAsset() override = default;

        [[nodiscard]] constexpr AssetType getType() const override {
            return AssetType::MATERIAL;
        };

        [[nodiscard]] MaterialData getData() const;

        void setDiffuseTexture(ImagePtr image);

        [[nodiscard]] Image getImage() const;

       private:
        void load() override;

        renderer_pipeline_id_t pipelineId_{PR_PIPELINE_DEFAULT_3D};

        ImagePtr diffuseImage_;

        // TODO: Replace these placeholders with real props
        double lightingProp1_{};
        double lightingProp2_{};
        double lightingProp3_{};
    };

    using MaterialPtr = AssetPtr<MaterialAsset>;
}  // namespace Presto
