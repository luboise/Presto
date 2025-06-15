#pragma once

#include "Presto/Assets/Asset.h"
#include "Presto/Assets/ImageData.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
class ImageAsset final : public Asset {
    friend class RenderingManager;

   public:
    ImageAsset(Presto::string name, Presto::ImageData image)
        : Asset(std::move(name)), image_(std::move(image)) {};

    ~ImageAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::IMAGE;
    };

    [[nodiscard]] const Presto::ImageData& data() const { return image_; }

    [[nodiscard]] std::size_t width() const { return image_.width; }
    [[nodiscard]] std::size_t height() const { return image_.height; }
    [[nodiscard]] auto getData() const -> const ImageData::image_data_t& {
        return image_.bytes;
    }

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] std::size_t size() const { return image_.size(); };

   private:
    bool load() override;

    Presto::ImageData image_;
};

}  // namespace Presto
