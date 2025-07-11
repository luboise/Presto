module;
#include <stb_image.h>

export module presto.internal.loading.image_loader;

import std;

import presto.assets.image;
import presto.assets.asset;
import presto.core;
import presto.utils;

export namespace Pr {

class ImageLoader {
   public:
    std::variant<Pr::ImageData, Pr::string> load(AssetArg filepath) {
        ByteArray data{Utils::File::ReadBinaryFile(filepath)};

        int x{};
        int y{};
        int channels{};

        // 4 desired channels because we want all images to be RGBA format
        constexpr int desired_channels = 4;

        // TODO: Remove the reinterpret casts from this function
        unsigned char* casted_data{
            reinterpret_cast<unsigned char*>(data.data())};

        auto* image_data{
            stbi_load_from_memory(casted_data, static_cast<int>(data.size()),
                                  &x, &y, &channels, desired_channels)};

        if (image_data == nullptr) {
            auto str{std::format("Unable to load image from path {}",
                                 filepath.path().string())};
            Pr::CoreLog(ERROR, str);

            return str;
        }

        ImageData new_image{.width = static_cast<size_t>(x),
                            .height = static_cast<size_t>(y),
                            .bytes{}};

        new_image.bytes.resize(new_image.size());

        // Get the image data
        std::span<std::byte> src_span(reinterpret_cast<std::byte*>(image_data),
                                      new_image.bytes.size());

        new_image.bytes =
            std::vector<std::byte>(src_span.begin(), src_span.end());

        stbi_image_free(image_data);

        new_image.width = x;
        new_image.height = y;

        return new_image;
    };
};

}  // namespace Pr

module :private;
#define STB_IMAGE_IMPLEMENTATION
