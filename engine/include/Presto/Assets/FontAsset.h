
#pragma once

#include "Presto/Assets/Asset.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
class FontAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit FontAsset(PR_STRING_ID name);

    ~FontAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::FONT;
    };

   private:
    bool load() override;
};

}  // namespace Presto
