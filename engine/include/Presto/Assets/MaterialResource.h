#pragma once

#include "Presto/Rendering/MaterialData.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/Image.h"
#include "Presto/Assets/ImageAsset.h"

namespace Presto {
    class MaterialAsset final : public Asset {
       public:
        friend class RenderingManager;

        explicit MaterialAsset(PR_STRING_ID name, ImageAsset* image = nullptr);
        ~MaterialAsset() override = default;

        [[nodiscard]] constexpr AssetType getType() const override {
            return AssetType::MATERIAL;
        };

        [[nodiscard]] MaterialData getData() const;

        void setImage(ImageAsset* image);

        [[nodiscard]] Image getImage() const;

       private:
        void load() override;

        ImageAsset* diffuseImage_;
    };
}  // namespace Presto
