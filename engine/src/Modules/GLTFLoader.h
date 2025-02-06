#include "ModelLoader.h"

namespace Presto {

class GLTFLoader : public ModelLoader {
   public:
    ImportedModelData load(
        const AssetPath& filepath,
        const std::vector<asset_name_t>& customNames) override {
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
};

}  // namespace Presto
