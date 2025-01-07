#include "Presto/Modules/ResourceManager.h"
#include "Presto/Utils/File.h"
#include "Rendering/Meshes/Cube.h"

#include "tiny_gltf.h"

// #include "Presto/Rendering/Mesh.h"

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

    AccessorData getDataFromAccessor(const tinygltf::Model& model,
                                     size_t accessorIndex) {
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

        auto bv = model.bufferViews[accessor.bufferView];
        auto buffer = model.buffers[bv.buffer];

        IndexList indices(accessor.count);

        std::vector<std::byte> ret_buffer(bv.byteLength);

        memcpy(ret_buffer.data(), buffer.data.data(), bv.byteLength);

        AccessorData ret{.buffer = ret_buffer, .count = accessor.count};

        return ret;
    }

    template <typename T>
    std::vector<T> castBuffer(const std::vector<std::byte>& bytes) {
        constexpr size_t read_size = sizeof(T);

        auto read_count = bytes.size() / read_size;
        std::vector<T> ret(read_count);

        for (int i = 0; i < read_count; i++) {
            std::memcpy(&ret[i], &ret[i * read_size], read_size);
        }

        return ret;
    }

    MeshResource& ResourceManager::LoadMeshFromDisk(const AssetPath& filepath) {
        // TODO: Implement the actual asset reading of GITF files. For now, this
        // is just stubbed with a unit cube at 0,0,0.

        fs::path filename{filepath.stem()};
        fs::path file_extension = filepath.extension();

        auto resource = std::make_unique<MeshResource>();

        if (file_extension == ".gltf") {
            auto data = Utils::File::ReadFile(filepath);

            TinyGLTF loader;
            Model model;
            std::string err;
            std::string warn;

            /*
bool ret = loader.LoadASCIIFromString(
    &model, &err, &warn, data.data(), data.size(),
    Utils::File::getFullPath(filepath).parent_path());
                    */

            fs::path full_asset_path = Utils::File::getFullPath(filepath);

            bool ret =
                loader.LoadASCIIFromFile(&model, &err, &warn, full_asset_path);

            PR_CORE_ASSERT(ret, std::string("Failed to read asset ") +
                                    full_asset_path.string());

            tinygltf::Mesh& mesh = model.meshes[0];
            tinygltf::Primitive& primitive = mesh.primitives[0];

            AccessorData index_data =
                getDataFromAccessor(model, primitive.indices);
            resource->indices.resize(index_data.buffer.size() /
                                     sizeof(glm::float32_t));

            AccessorData pos_data =
                getDataFromAccessor(model, primitive.attributes["POSITION"]);

            AccessorData normals_data =
                getDataFromAccessor(model, primitive.attributes["NORMAL"]);

            AccessorData texcoords_data =
                getDataFromAccessor(model, primitive.attributes["TEXCOORD_0"]);

            resource->indices =
                castBuffer<MeshResource::IndexType>(index_data.buffer);
            resource->index_count = resource->indices.size();

            resource->positions =
                castBuffer<MeshResource::PositionType>(pos_data.buffer);
            resource->draw_mode = primitive.mode;

            resource->name = mesh.name;
        }

        /*
auto* new_mr{new MeshResource()};

auto default_cube = Meshes::Cube({0, 0, 0}, 1, {1, 1, 1});

new_mr->name = filename;
new_mr->vertices = default_cube.vertices;
new_mr->indices = default_cube.indices;
        */

        auto key = resource->name;
        meshResources_[key] = std::move(resource);
        return *meshResources_[key];
    }

    void ResourceManager::Init() {
        PR_CORE_INFO("Initialising ResourceManager.");
        instance_ = std::unique_ptr<ResourceManager>(new ResourceManager());
    }

}  // namespace Presto
