export module presto.internal.importing;

import presto.core.types;

export namespace Pr {

struct ImportedVertexAttribute {
    Pr::string name;

    ShaderDataType type;
    Pr::size_t count;

    ByteArray data;

    [[nodiscard]] Pr::size_t dataSize() const;
};

using ImportedAttributeList = std::vector<ImportedVertexAttribute>;

constexpr Pr::uint8_t PR_NO_MATERIAL_INDEX = -1;

struct ImportedMaterial;

struct ImportedMesh {
    ImportedAttributeList attributes;

    Pr::uint8_t material_index{PR_NO_MATERIAL_INDEX};

    Pr::size_t vertex_count;
    IndexList indices;

    MeshDrawMode draw_mode;

    [[nodiscard]] bool hasMaterial() const {
        return material_index != PR_NO_MATERIAL_INDEX;
    };
};

struct ImportedModel {
    Pr::string name;

    std::vector<ImportedMesh> meshes;
};

struct ImportedMaterialProperty {
    uniform_name_t name;

    UniformVariableType data_type;
    ErasedBytes data;
};

struct ImportedMaterial {
    Pr::string name;
    MaterialType material_type{MaterialType::DEFAULT_3D};

    std::vector<ImportedMaterialProperty> values;
};

struct ImportedTexture {
    Pr::string name;
    Pr::ImageData image;
};

struct ImportedModelData {
    std::vector<ImportedModel> models;

    std::vector<ImportedMaterial> materials;
    std::vector<ImportedTexture> textures;
};

class ModelLoader {
   public:
    virtual ~ModelLoader() = default;

    virtual ImportedModelData load(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames) = 0;
};

}  // namespace Pr
