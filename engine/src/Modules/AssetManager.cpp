#include "Modules/AssetManager.h"

#include "Presto/Assets/Asset.h"
#include "Presto/Assets/ImageAsset.h"

#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Utils/File.h"
// #include "Rendering/Meshes/Cube.h"

#include "GLTFLoader.h"

#include "Modules/RenderingManager.h"

#include <stb_image.h>
#include <memory>

namespace Presto {

MaterialPtr AssetManager::createMaterialFromImport(
    const ImportedMaterial& imported_material,
    std::vector<TexturePtr>& texturePtrs) {
    MaterialPtr material{RenderingManager::get().createMaterial(
        MaterialType::DEFAULT_3D, imported_material.name)};

    for (const auto& value : imported_material.values) {
        if (value.data_type == UniformVariableType::TEXTURE) {
            auto texture_index{
                value.data.as<ImportTypeOf<UniformVariableType::TEXTURE>>()};

            material->setProperty(value.name, texturePtrs[texture_index]);
        } else {
            material->setProperty(value.name, value.data);
        }
    }

    return material;
};

std::vector<ModelPtr> AssetManager::loadModelsFromDisk(
    const AssetArg& filepath, const std::vector<asset_name_t>& customNames) {
    std::string filename{filepath.basename()};
    fs::path file_extension{filepath.fileExtension()};

    ImportedModelData imported_data;

    if (file_extension == ".gltf" || file_extension == ".glb") {
        GLTFLoader loader;
        imported_data = loader.load(filepath, customNames);
    }

    PR_ASSERT(
        imported_data.models.size() > 0,
        std::format("No models could be found in {}.", filepath.string()));

    std::vector<ModelPtr> new_model_ptrs(imported_data.models.size());
    std::vector<MaterialPtr> new_material_ptrs(imported_data.materials.size());
    std::vector<TexturePtr> new_texture_ptrs(imported_data.textures.size());

    // Turn the textures into assets
    for (std::size_t i{0}; i < imported_data.textures.size(); ++i) {
        ImportedTexture& texture{imported_data.textures[i]};
        if (texture.name.empty()) {
            PR_WARN(
                "Imported texture ({}x{}) has no name. Skipping this "
                "import.",
                texture.image.width, texture.image.height);
            continue;
        }

        // TODO: Make this cache images that it gets from the import rather than
        // create a new asset for each one
        const auto& image_ptr{createImageAsset(texture.name, texture.image)};

        new_texture_ptrs[i] =
            RenderingManager::get().createTexture2D(image_ptr);
    }

    // Turn the materials into assets
    for (std::size_t i{0}; i < new_material_ptrs.size(); ++i) {
        new_material_ptrs[i] = createMaterialFromImport(
            imported_data.materials[i], new_texture_ptrs);
    }

    // Turns the models into model assets (and subsequently their underlying
    // meshes)

    for (size_t i{0}; i < imported_data.models.size(); ++i) {
        const auto& imported_model = imported_data.models[i];

        auto new_name{i >= (customNames.size() - 1) ? customNames[i]
                                                    : imported_model.name};

        ModelPtr model{std::make_shared<ModelAsset>(new_name)};

        const auto* pipeline_structure{
            RenderingManager::get().getPipelineStructure(
                PR_PIPELINE_DEFAULT_3D)};
        PR_CORE_ASSERT(pipeline_structure != nullptr,
                       "The pipeline structure of the default pipelines must "
                       "not be nullptr.");

        for (const auto& imported_mesh : imported_model.meshes) {
            MeshPtr mesh{std::make_shared<MeshAsset>()};

            mesh->setVertices(imported_mesh.attributes)
                .setIndices(imported_mesh.indices);

            // Make sure the material is loaded
            if (imported_mesh.hasMaterial() &&
                new_material_ptrs[imported_mesh.material_index] == nullptr) {
                mesh->setDefaultMaterial(
                    new_material_ptrs[imported_mesh.material_index]);
            }
            model->meshes_.push_back(mesh);
        }

        assets_[AssetType::MODEL][new_name] = model;
        new_model_ptrs.push_back(model);
    }

    // TODO: Implement full path/cwd system for engine to find it at
    // runtime, or have the user change it (would help the editor)
    // fs::path full_asset_path = Utils::File::getFullPath(filepath);
    return new_model_ptrs;
};

ImagePtr AssetManager::loadImageFromDisk(const AssetArg& filepath,
                                         const asset_name_t& customName) {
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
        PR_ERROR("Unable to load image from path {}", filepath.path().string());
        return nullptr;
    }

    Image new_image{.width = static_cast<size_t>(x),
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

ImagePtr AssetManager::createImageAsset(const asset_name_t& customName,
                                        const Presto::Image& image) {
    auto new_image{std::make_shared<ImageAsset>(customName, image)};
    assets_[AssetType::IMAGE][customName] = new_image;

    return new_image;
};

MaterialDefinitionPtr AssetManager::createMaterialDefinition(
    Presto::string name, const PipelineStructure& structure) {
    if (assets_[AssetType::MATERIAL_DEFINITION].contains(name)) {
        PR_ERROR(
            "Unable to create material definition, as a definition with the "
            "same name already exists.");
        return nullptr;
    }

    MaterialDefinitionPtr new_definition{
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

}  // namespace Presto
