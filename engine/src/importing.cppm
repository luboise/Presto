export module presto.internal.importing;

export namespace Presto {

struct ImportedVertexAttribute {
    Presto::string name;

    ShaderDataType type;
    Presto::size_t count;

    ByteArray data;

    [[nodiscard]] Presto::size_t dataSize() const;
};

using ImportedAttributeList = std::vector<ImportedVertexAttribute>;

static constexpr Presto::uint8_t PR_NO_MATERIAL_INDEX = -1;

struct ImportedMaterial;

struct ImportedMesh {
    ImportedAttributeList attributes;

    Presto::uint8_t material_index{PR_NO_MATERIAL_INDEX};

    Presto::size_t vertex_count;
    IndexList indices;

    MeshDrawMode draw_mode;

    [[nodiscard]] bool hasMaterial() const {
        return material_index != PR_NO_MATERIAL_INDEX;
    };
};

struct ImportedModel {
    Presto::string name;

    std::vector<ImportedMesh> meshes;
};

struct ImportedMaterialProperty {
    uniform_name_t name;

    UniformVariableType data_type;
    ErasedBytes data;
};

struct ImportedMaterial {
    Presto::string name;
    MaterialType material_type{MaterialType::DEFAULT_3D};

    std::vector<ImportedMaterialProperty> values;
};

struct ImportedTexture {
    Presto::string name;
    Presto::ImageData image;
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

}  // namespace Presto
