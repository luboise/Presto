export module presto.assets.image;

import std;

import presto.types.core;
import presto.assets.types;

export namespace Pr {
struct ImageData {
    using image_data_t = ByteArray;

    Pr::size_t width;
    Pr::size_t height;
    image_data_t bytes;

    static constexpr auto PIXEL_BYTE_LENGTH = 4;

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] Pr::size_t size() const {
        return width * height * PIXEL_BYTE_LENGTH;
    };
};

class ImageAsset final : public Asset {
   public:
    ImageAsset(Pr::string name, Pr::ImageData image)
        : Asset(std::move(name)), image_(std::move(image)) {};

    ~ImageAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::IMAGE;
    };

    [[nodiscard]] const Pr::ImageData& data() const { return image_; }

    [[nodiscard]] Pr::size_t width() const { return image_.width; }
    [[nodiscard]] Pr::size_t height() const { return image_.height; }
    [[nodiscard]] auto getData() const -> const ImageData::image_data_t& {
        return image_.bytes;
    }

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] Pr::size_t size() const { return image_.size(); };

   private:
    bool load() override;

    Pr::ImageData image_;
};

}  // namespace Pr
