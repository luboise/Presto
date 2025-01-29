#include "Presto/Modules/AssetManager.h"

#include "Presto/Assets/ImageAsset.h"

#include "Presto/Utils/File.h"
#include "Rendering/Meshes/Cube.h"

#include "tiny_gltf.h"

// NOT NEEDED SINCE TINY_GLTF USES IT
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace tinygltf;

namespace Presto {
    // const fs::path executableDirectory =
    // fs::absolute(fs::path(argv[0])).parent_path();

    // TODO: Fix this to be based on the executable path rather than the
    // directory which the program was called from

    struct AccessorData {
        std::vector<std::byte> buffer;
        size_t count{static_cast<size_t>(-1)};
    };

    void AssetManager::init() {
        PR_CORE_INFO("Initialising AssetManager.");
        instance_ = std::unique_ptr<AssetManager>(new AssetManager());
    }

    void AssetManager::shutdown() { instance_->assets_.clear(); }

    AccessorData getDataFromAccessor(const tinygltf::Model& model,
                                     size_t accessorIndex) {
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

        const auto& bv = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bv.buffer];

        IndexList indices(accessor.count);

        std::vector<std::byte> ret_buffer(bv.byteLength);

        auto accessor_offset = bv.byteOffset + accessor.byteOffset;

        std::memcpy(ret_buffer.data(), buffer.data.data() + accessor_offset,
                    bv.byteLength);

        AccessorData ret{.buffer = ret_buffer, .count = accessor.count};

        return ret;
    }

    template <typename T>
    std::vector<T> getDataFromAccessor2(const tinygltf::Model& model,
                                        size_t accessorIndex) {
        constexpr size_t read_size = sizeof(T);

        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

        const auto& bv = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bv.buffer];

        const auto read_count = accessor.count;

        std::vector<T> ret(read_count);

        auto accessor_offset = bv.byteOffset + accessor.byteOffset;
        auto stride = bv.byteStride;
        if (stride == 0) {
            stride = read_size;
        };

        // for (int byte = accessor_offset; byte < read_count; byte++) {
        for (size_t i = 0; i < read_count; i++) {
            std::memcpy(&ret[i], &buffer.data[accessor_offset + i * stride],
                        read_size);
        }

        return ret;
    }

    template <typename T>
    std::vector<T> castBuffer(const std::vector<std::byte>& bytes) {
        constexpr size_t read_size = sizeof(T);

        auto read_count = bytes.size() / read_size;
        std::vector<T> ret(read_count);

        for (int i = 0; i < read_count; i++) {
            std::memcpy(&ret[i], &bytes[i * read_size], read_size);
        }

        return ret;
    }

    std::vector<ModelPtr> AssetManager::loadModelsFromDisk(
        const AssetPath& filepath,
        const std::vector<asset_name_t>& customNames) {
        fs::path filename{filepath.stem()};
        fs::path file_extension = filepath.extension();

        std::vector<ModelPtr> new_models;
        std::vector<MaterialPtr> new_material_ptrs;

        if (file_extension == ".gltf" || file_extension == ".glb") {
            TinyGLTF loader;
            Model model;
            std::string err;
            std::string warn;

            // TODO: Implement full path/cwd system for engine to find it at
            // runtime, or have the user change it (would help the editor)
            // fs::path full_asset_path = Utils::File::getFullPath(filepath);
            const fs::path& full_asset_path = filepath;

            bool ret{false};

            if (file_extension == ".gltf") {
                ret = loader.LoadASCIIFromFile(&model, &err, &warn,
                                               full_asset_path);
            } else {
                ret = loader.LoadBinaryFromFile(&model, &err, &warn,
                                                full_asset_path);
            }

            PR_CORE_ASSERT(ret, std::string("Failed to read asset ") +
                                    full_asset_path.string());

            for (auto& material : model.materials) {
                new_material_ptrs.push_back(
                    std::make_shared<MaterialAsset>(material.name));
                auto new_material{this->createMaterial(material.name)};

                new_material->name_ = material.name;

                auto texture_index =
                    material.pbrMetallicRoughness.baseColorTexture.index;

                if (texture_index != -1) {
                    const auto& texture = model.textures[texture_index];
                    const auto& image_data = model.images[texture.source];

                    Presto::Image image{
                        .width = static_cast<size_t>(image_data.width),
                        .height = static_cast<size_t>(image_data.height)};

                    image.bytes.resize(image.size());

                    std::memcpy(image.bytes.data(), image_data.image.data(),
                                image.bytes.size());

                    ImagePtr image_resource{
                        createImageAsset(texture.name, image)};

                    new_material->setDiffuseTexture(image_resource);

                    // TODO: Maybe make this load when an entity enters the
                    // scene, or at the end of every update loop
                    new_material->ensureLoaded();
                }
            }

            for (size_t i = 0; i < model.meshes.size(); i++) {
                auto& mesh = model.meshes[i];
                auto new_name{i >= (customNames.size() - 1) ? customNames[i]
                                                            : mesh.name};

                auto new_model{std::make_shared<ModelAsset>(new_name)};

                for (tinygltf::Primitive& primitive : mesh.primitives) {
                    RawMeshData new_submesh;

                    new_submesh.draw_mode = primitive.mode;

                    new_submesh.indices =
                        getDataFromAccessor2<RawMeshData::IndexType>(
                            model, primitive.indices);

                    new_submesh.positions =
                        getDataFromAccessor2<RawMeshData::PositionType>(
                            model, primitive.attributes["POSITION"]);

                    new_submesh.normals =
                        getDataFromAccessor2<RawMeshData::NormalType>(
                            model, primitive.attributes["NORMAL"]);

                    new_submesh.tex_coords =
                        getDataFromAccessor2<RawMeshData::TexCoordsType>(
                            model, primitive.attributes["TEXCOORD_0"]);

                    // Turn the raw mesh data into a new mesh asset
                    MeshData data{MeshData::from(new_submesh)};
                    auto new_mesh_asset{
                        std::make_shared<MeshAsset>(std::move(data))};

                    new_mesh_asset->setDefaultMaterial(
                        new_material_ptrs[primitive.material]);
                    new_model->meshes_.push_back(new_mesh_asset);
                }

                new_model->name_ = new_name;

                assets_[AssetType::MODEL][new_name] = new_model;

                new_models.push_back(new_model);
            }

            for (auto& mat : new_material_ptrs) {
                assets_[AssetType::MATERIAL].emplace(mat->name_,
                                                     std::move(mat));
            }
        }

        return new_models;
    };

    MaterialPtr AssetManager::createMaterial(const asset_name_t& customName) {
        auto new_material{std::make_shared<MaterialAsset>(customName)};
        new_material->name_ = customName;

        auto key = new_material->name_;
        assets_[AssetType::MATERIAL][key] = new_material;

        return new_material;
    }

    ImagePtr AssetManager::loadImageFromDisk(const AssetPath& filepath,
                                             const asset_name_t& customName) {
        fs::path filename{filepath.stem()};
        fs::path file_extension = filepath.extension();

        std::vector<std::byte> data = Utils::File::ReadBinaryFile(filepath);

        int x{};
        int y{};
        int channels{};

        // 4 desired channels because we want all images to be RGBA format
        constexpr int desired_channels = 4;

        unsigned char* casted_data{
            reinterpret_cast<unsigned char*>(data.data())};

        auto* image_data =
            stbi_load_from_memory(casted_data, static_cast<int>(data.size()),
                                  &x, &y, &channels, desired_channels);

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

        new_image.bytes =
            std::vector<uint8_t>(src_span.begin(), src_span.end());

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

        auto key = new_resource->name();

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

}  // namespace Presto
