#include "Presto/Modules/ResourceManager.h"
#include "Presto/Resources/ImageResource.h"
#include "Presto/Utils/File.h"
#include "Rendering/Meshes/Cube.h"

#include "tiny_gltf.h"

// NOT NEEDED SINCE TINY_GLTF USES IT
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// #include "Presto/Rendering/MeshGroup.h"

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

    void ResourceManager::init() {
        PR_CORE_INFO("Initialising ResourceManager.");
        instance_ = std::unique_ptr<ResourceManager>(new ResourceManager());
    }

    void ResourceManager::shutdown() { instance_->resources_.clear(); }

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

    std::vector<ModelAsset*> ResourceManager::loadMeshesFromDisk(
        const AssetPath& filepath, const resource_name_t& customName) {
        fs::path filename{filepath.stem()};
        fs::path file_extension = filepath.extension();

        std::vector<ModelAsset*> new_meshes;
        std::vector<std::unique_ptr<MaterialAsset>> new_materials;

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
                new_materials.push_back(
                    std::make_unique<MaterialResource>(material.name));

                auto& new_material = *new_materials.back();

                new_material.name_ = material.name;

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

                    ImageResource* image_resource{
                        createImageResource(texture.name, image)};

                    new_material.setImage(image_resource);

                    // TODO: Maybe make this load when an entity enters the
                    // scene, or at the end of every update loop
                    new_material.ensureLoaded();
                }
            }

            for (auto& mesh : model.meshes) {
                auto new_mesh = std::make_unique<MeshResource>(mesh.name);

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

                    new_submesh.material_data =
                        new_materials[primitive.material].get()->getData();

                    new_mesh->meshes_.push_back(new_submesh);
                }

                auto key = customName.empty() ? mesh.name : customName;
                new_mesh->name_ = key;

                resources_[ResourceType::MESH][key] = std::move(new_mesh);

                new_meshes.push_back(resources_[ResourceType::MESH][key]
                                         .get()
                                         ->as<MeshResource>());
            }

            for (auto& mat : new_materials) {
                resources_[ResourceType::MATERIAL].emplace(mat->name_,
                                                           std::move(mat));
            }
        }

        return new_meshes;
    }

    MaterialAsset& ResourceManager::createMaterial(
        const resource_name_t& customName) {
        auto resource = std::make_unique<MaterialAsset>(customName);
        resource->name_ = customName;

        auto key = resource->name_;
        resources_[ResourceType::MATERIAL][key] = std::move(resource);
        return *(
            resources_[ResourceType::MATERIAL][key]->as<MaterialResource>());
    }

    ImageAsset& ResourceManager::loadImageFromDisk(
        const AssetPath& filepath, const resource_name_t& customName) {
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

        // Get the image data
        std::span<unsigned char> src_span(image_data, data.size());

        Image new_image{
            .width = static_cast<size_t>(x),
            .height = static_cast<size_t>(y),
            .bytes = std::vector<uint8_t>(src_span.begin(), src_span.end())};

        stbi_image_free(image_data);

        new_image.width = x;
        new_image.height = y;

        auto new_resource{std::make_unique<ImageAsset>(customName, new_image)};

        /*
    auto* new_mr{new MeshResource()};

    auto default_cube = Meshes::Cube({0, 0, 0}, 1, {1, 1, 1});

    new_mr->name = filename;
    new_mr->vertices = default_cube.vertices;
    new_mr->indices = default_cube.indices;
        */

        auto key = new_resource->name();

        resources_[ResourceType::IMAGE][key] = std::move(new_resource);
        return *(resources_[ResourceType::IMAGE][key]->as<ImageResource>());
    }

    /*
MeshResource* ResourceManager::getMesh(const resource_name_t& key) const {
    if (auto found = meshResources_.find(key);
        found != meshResources_.end()) {
        return found->second.get();
    }

    return nullptr;
}

ImageResource* ResourceManager::getImage(const resource_name_t& key) const {
    if (auto found = imageResources_.find(key);
        found != imageResources_.end()) {
        return found->second.get();
    }

    return nullptr;
}

MaterialResource* ResourceManager::getMaterial(
    const resource_name_t& key) const {
    if (auto found = materialResources_.find(key);
        found != materialResources_.end()) {
        return found->second.get();
    }

    return nullptr;
}
    */

    ImageAsset* ResourceManager::createImageResource(
        const resource_name_t& customName, const Presto::Image& image) {
        resources_[ResourceType::IMAGE][customName] =
            std::make_unique<ImageResource>(customName, image);

        return resources_[ResourceType::IMAGE][customName]->as<ImageResource>();
    };
}  // namespace Presto
