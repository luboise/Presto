#include "Presto/Modules/AssetManager.h"

#include "Presto/Assets/ImageAsset.h"

#include "Presto/Utils/File.h"
// #include "Rendering/Meshes/Cube.h"

#include "GLTFLoader.h"

namespace Presto {
// const fs::path executableDirectory =
// fs::absolute(fs::path(argv[0])).parent_path();

// TODO: Fix this to be based on the executable path rather than the
// directory which the program was called from

std::vector<ModelPtr> AssetManager::loadModelsFromDisk(
    const AssetPath& filepath, const std::vector<asset_name_t>& customNames) {
    std::string filename{filepath.stem().string()};
    fs::path file_extension = filepath.extension();

    std::vector<ModelPtr> new_model_ptrs;
    std::vector<MaterialPtr> new_material_ptrs;
    std::vector<ImagePtr> new_image_ptrs;

    ImportedModelData imported_data;

    if (file_extension == ".gltf" || file_extension == ".glb") {
        GLTFLoader loader;
        imported_data = loader.load(filepath, customNames);
    }

    PR_ASSERT(
        imported_data.models.size(),
        std::format("No models could be found in {}.", filepath.string()));

    for (size_t i = 0; i < imported_data.images.size(); i++) {
        ImagePtr image{createImageAsset(std::format("{}_image_{}", filename, i),
                                        imported_data.images[i])};
        new_image_ptrs.push_back(image);
    }

    for (size_t i = 0; i < imported_data.materials.size(); i++) {
        const ImportedMaterial& imported_material{imported_data.materials[i]};

        auto material_name{std::format("{}_material_{}", filename, i)};
        MaterialPtr new_material{
            createMaterial(material_name, imported_material)};

        // TODO: Maybe make this load when an entity enters the
        // scene, or at the end of every update loop
        new_material->ensureLoaded();

        new_material_ptrs.push_back(new_material);
    }

    new_material_ptrs.push_back();

    for (size_t i{0}; i < imported_data.models.size(); ++i) {
        const auto& model = imported_data.models[i];
        auto new_name{i >= (customNames.size() - 1) ? customNames[i]
                                                    : model.name};

        auto new_model{std::make_shared<ModelAsset>(new_name)};

        for (const ImportedMesh& mesh : model.meshes) {
            // Turn the raw mesh data into a new mesh asset
            auto new_mesh_asset{std::make_shared<MeshAsset>(mesh)};
            new_mesh_asset->setDefaultMaterial(new_materials[mesh.material]);

            new_model->meshes_.push_back(new_mesh_asset);
        }

        new_model->name_ = new_name;

        assets_[AssetType::MODEL][new_name] = new_model;

        new_model_ptrs.push_back(new_model);
    }

    // TODO: Implement full path/cwd system for engine to find it at
    // runtime, or have the user change it (would help the editor)
    // fs::path full_asset_path = Utils::File::getFullPath(filepath);

    for (ImportedMaterial& material : imported_data.materials) {
        new_material_ptrs.push_back(
            std::make_shared<MaterialAsset>(material.name));
        auto new_material{this->createMaterial(material.name)};

        new_material->name_ = material.name;

        auto texture_index =
            material.pbrMetallicRoughness.baseColorTexture.index;

        if (texture_index != -1) {
            const auto& texture = model.textures[texture_index];
            const auto& image_data = imported_data.images[texture.source];

            Presto::Image image{
                .width = static_cast<size_t>(image_data.width),
                .height = static_cast<size_t>(image_data.height)};

            image.bytes.resize(image.size());

            std::memcpy(image.bytes.data(), image_data.image.data(),
                        image.bytes.size());

            ImagePtr image_resource{createImageAsset(texture.name, image)};

            new_material->setDiffuseTexture(image_resource);

            // TODO: Maybe make this load when an entity enters the
            // scene, or at the end of every update loop
            new_material->ensureLoaded();
        }
    }

    for (size_t i = 0; i < model.meshes.size(); i++) {
    }

    for (auto& mat : new_material_ptrs) {
        assets_[AssetType::MATERIAL].emplace(mat->name_, std::move(mat));
    }

    return new_model_ptrs;
};

MaterialPtr AssetManager::createMaterial(const asset_name_t& customName,
                                         MaterialStructure inStructure) {
    auto new_material{std::make_shared<MaterialAsset>(customName, inStructure)};
    new_material->name_ = customName;

    const auto key = new_material->name_;
    assets_[AssetType::MATERIAL][key] = new_material;

    return new_material;
}

ImagePtr AssetManager::loadImageFromDisk(const AssetPath& filepath,
                                         const asset_name_t& customName) {
    fs::path filename{filepath.stem()};
    fs::path file_extension = filepath.extension();

    ByteArray data = Utils::File::ReadBinaryFile(filepath);

    int x{};
    int y{};
    int channels{};

    // 4 desired channels because we want all images to be RGBA format
    constexpr int desired_channels = 4;

    unsigned char* casted_data{reinterpret_cast<unsigned char*>(data.data())};

    auto* image_data =
        stbi_load_from_memory(casted_data, static_cast<int>(data.size()), &x,
                              &y, &channels, desired_channels);

    if (image_data == nullptr) {
        PR_ERROR("Unable to load image from path {}", filepath.string());
        return nullptr;
    }

    Image new_image{.width = static_cast<size_t>(x),
                    .height = static_cast<size_t>(y),
                    .bytes{}};

    new_image.bytes.resize(new_image.size());

    // Get the image data
    std::span<unsigned char> src_span(image_data, new_image.bytes.size());

    new_image.bytes = std::vector<uint8_t>(src_span.begin(), src_span.end());

    stbi_image_free(image_data);

    new_image.width = x;
    new_image.height = y;

    auto new_resource{std::make_shared<ImageAsset>(customName, new_image)};

    /*
auto* new_mr{new ModelAsset()};

auto default_cube = Meshes::Cube({0, 0, 0}, 1, {1, 1, 1});

new_mr->name = filename;
new_mr->vertices = default_cube.vertices;
new_mr->indices = default_cube.indices;
    */

    const auto key = new_resource->name();

    assets_[AssetType::IMAGE][key] = new_resource;

    return new_resource;
}

/*
ModelAsset* AssetManager::getMesh(const resource_name_t& key) const {
if (auto found = meshAssets_.find(key);
    found != meshAssets_.end()) {
    return found->second.get();
}

return nullptr;
}

ImagePtr AssetManager::getImage(const resource_name_t& key) const {
if (auto found = imageAssets_.find(key);
    found != imageAssets_.end()) {
    return found->second.get();
}

return nullptr;
}

MaterialAsset* AssetManager::getMaterial(
const resource_name_t& key) const {
if (auto found = materialAssets_.find(key);
    found != materialAssets_.end()) {
    return found->second.get();
}

return nullptr;
}
*/

ImagePtr AssetManager::createImageAsset(const asset_name_t& customName,
                                        const Presto::Image& image) {
    auto new_image{std::make_shared<ImageAsset>(customName, image)};
    assets_[AssetType::IMAGE][customName] = new_image;

    return new_image;
};

MaterialInstancePtr AssetManager::createMaterialInstance(
    const asset_name_t& customName, const MaterialPtr& material) {
    auto new_instance{
        std::make_shared<MaterialInstanceAsset>(customName, material)};
    assets_[AssetType::MATERIAL_INSTANCE][customName] = new_instance;

    return new_instance;
};

}  // namespace Presto
