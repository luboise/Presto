export module presto.internal.managers.asset_manager:image;

import presto.assets;

namespace Pr {

Pr::Ptr<Pr::ImageAsset> AssetManager::loadImageFromDisk(
    const AssetArg& filepath, const asset_name_t& customName) {
    ByteArray data{Utils::File::ReadBinaryFile(filepath)};

    int x{};
    int y{};
    int channels{};

    // 4 desired channels because we want all images to be RGBA format
    constexpr int desired_channels = 4;

    // TODO: Remove the reinterpret casts from this function
    unsigned char* casted_data{reinterpret_cast<unsigned char*>(data.data())};

    auto* image_data{stbi_load_from_memory(casted_data,
                                           static_cast<int>(data.size()), &x,
                                           &y, &channels, desired_channels)};

    if (image_data == nullptr) {
        Pr::CoreLog(ERROR, "Unable to load image from path {}",
                    filepath.path().string());
        return nullptr;
    }

    ImageData new_image{.width = static_cast<size_t>(x),
                        .height = static_cast<size_t>(y),
                        .bytes{}};

    new_image.bytes.resize(new_image.size());

    // Get the image data
    std::span<std::byte> src_span(reinterpret_cast<std::byte*>(image_data),
                                  new_image.bytes.size());

    new_image.bytes = std::vector<std::byte>(src_span.begin(), src_span.end());

    stbi_image_free(image_data);

    new_image.width = x;
    new_image.height = y;

    auto new_resource{std::make_shared<ImageAsset>(customName, new_image)};

    const auto key = new_resource->name();

    assets_[AssetType::IMAGE][key] = new_resource;

    return new_resource;
}

}  // namespace Pr
