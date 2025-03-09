#pragma once

#include "Presto/Assets/Image.h"

#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/CoreTypes.h"

#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Types/MaterialTypes.h"

namespace Presto {

class ErasedBytes;

struct ImportedVertexAttribute {
    PR_STRING_ID name;

    ShaderDataType type;
    Presto::size_t count;

    ByteArray data;

    [[nodiscard]] Presto::size_t dataSize() const;
};

using ImportedAttributeList = std::vector<ImportedVertexAttribute>;

static constexpr Presto::uint8_t PR_NO_MATERIAL_INDEX = -1;

struct ImportedMesh {
    ImportedAttributeList attributes;

    Presto::uint8_t material_index{PR_NO_MATERIAL_INDEX};

    Presto::size_t vertex_count;
    IndexList indices;

    // TODO: Make this a Presto type that can map to multiple graphics APIs
    std::int32_t draw_mode;

    [[nodiscard]] bool hasMaterial() const {
        return material_index != PR_NO_MATERIAL_INDEX;
    };
};

struct ImportedModel {
    PR_STRING_ID name;

    std::vector<ImportedMesh> meshes;
};

namespace DefaultAttributeName {

constexpr auto POSITION = "a_vertexPosition";
constexpr auto COLOUR = "a_vertexPosition";
constexpr auto NORMAL = "a_normal";
constexpr auto TEXCOORDS = "a_texcoords";

}  // namespace DefaultAttributeName

namespace DefaultMaterialPropertyName {

constexpr auto BASE_COLOUR = "u_baseColour";
// Alias for American english speakers
constexpr auto BASE_COLOR = BASE_COLOUR;

constexpr auto DIFFUSE_TEXTURE = "u_diffuseTexture";

constexpr auto UI_POSITION = "u_position";
constexpr auto UI_SIZE = "u_size";
constexpr auto UI_OFFSET = "u_offset";

};  // namespace DefaultMaterialPropertyName

struct ImportedMaterialProperty {
    uniform_name_t name;

    UniformVariableType data_type;
    ErasedBytes data;
};

struct ImportedMaterial {
    PR_STRING_ID name;
    MaterialType material_type{MaterialType::DEFAULT_3D};

    std::vector<ImportedMaterialProperty> values;
};

struct ImportedTexture {
    Presto::string name;
    Presto::Image image;
};

struct ImportedModelData {
    std::vector<ImportedModel> models;

    std::vector<ImportedMaterial> materials;
    std::vector<ImportedTexture> textures;
};

}  // namespace Presto
