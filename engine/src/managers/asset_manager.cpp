import module presto.internal.managers.asset_manager;

import std;

namespace Pr {

/*
Pr::Ptr<Pr::MaterialInstance> AssetManager::createMaterialFromImport(
const ImportedMaterial& imported_material,
std::vector<Pr::Ptr<Pr::Texture>>& texturePtrs) {
Pr::Ptr<Pr::MaterialInstance> material{RenderingManager::get().createMaterial(
MaterialType::DEFAULT_3D, imported_material.name)};

material->setFromImport(imported_material, texturePtrs);
return material;
};
*/

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

Pr::Ptr<Pr::MaterialAsset> AssetManager::createMaterialDefinition(
    Pr::string name, const PipelineStructure& structure) {
    if (assets_[AssetType::MATERIAL_DEFINITION].contains(name)) {
        Pr::CoreLog(
            ERROR,
            "Unable to create material definition, as a definition with the "
            "same name already exists.");
        return nullptr;
    }

    Pr::Ptr<Pr::MaterialAsset> new_definition{
        std::make_shared<MaterialAsset>(name, structure)};

    assets_[AssetType::MATERIAL_DEFINITION][name] = new_definition;

    return new_definition;
}

Ptr<MaterialAsset> AssetManager::getMaterialDefinition(pipeline_id_t id) {
    const auto map{assets_[AssetType::MATERIAL_DEFINITION] |
                   std::views::values};

    if (auto it{std::ranges::find_if(
            map,
            [id](const auto& y) -> bool {
                return y->template as<MaterialAsset>()->pipelineId() == id;
            })};
        it != map.end()) {
        return std::dynamic_pointer_cast<MaterialAsset>(it.base()->second);
    }

    return nullptr;
};

}  // namespace Pr
