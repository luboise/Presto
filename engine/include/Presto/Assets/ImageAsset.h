#pragma once

#include "Presto/Assets/Image.h"

#include "Presto/Rendering/RenderTypes.h"

#include "Presto/Assets/Asset.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
class ImageAsset final : public Asset {
    friend class RenderingManager;

    using image_data_t = std::vector<uint8_t>;

   public:
    ImageAsset(PR_STRING_ID name, Presto::Image image)
        : Asset(std::move(name)), image_(std::move(image)) {};

    ~ImageAsset() override = default;

    [[nodiscard]] constexpr AssetType getType() const override {
        return AssetType::IMAGE;
    };

    [[nodiscard]] const Presto::Image& getImage() const { return image_; }

    [[nodiscard]] std::size_t getWidth() const { return image_.width; }
    [[nodiscard]] std::size_t getHeight() const { return image_.height; }
    [[nodiscard]] const image_data_t& getData() const { return image_.bytes; }

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] std::size_t size() const { return image_.size(); };

    [[nodiscard]] renderer_texture_id_t getRenderId() const {
        return renderId_;
    };

   private:
    void load() override;

    Presto::Image image_;
    renderer_texture_id_t renderId_{UNREGISTERED_RENDER_DATA_ID};
};

using ImagePtr = AssetPtr<ImageAsset>;
}  // namespace Presto
