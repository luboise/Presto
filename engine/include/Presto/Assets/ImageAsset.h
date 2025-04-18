#pragma once

#include "Presto/Assets/Asset.h"
#include "Presto/Assets/Image.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
class ImageAsset final : public Asset {
    friend class RenderingManager;

   public:
    ImageAsset(PR_STRING_ID name, Presto::Image image)
        : Asset(std::move(name)), image_(std::move(image)) {};

    ~ImageAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::IMAGE;
    };

    [[nodiscard]] const Presto::Image& getImage() const { return image_; }

    [[nodiscard]] std::size_t getWidth() const { return image_.width; }
    [[nodiscard]] std::size_t getHeight() const { return image_.height; }
    [[nodiscard]] auto getData() const -> const Image::image_data_t& {
        return image_.bytes;
    }

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] std::size_t size() const { return image_.size(); };

   private:
    bool load() override;

    Presto::Image image_;
};

}  // namespace Presto
