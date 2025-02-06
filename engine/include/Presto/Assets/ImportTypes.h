#pragma once

#include "Presto/Assets/Image.h"

#include "Presto/Rendering/AttributeTypes.h"

#include "Presto/Rendering/MaterialData.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Rendering/VertexAttribute.h"
#include "Rendering/ErasedBytes.h"

namespace Presto {
struct ImportedMesh {
    ByteArray bytes;

    AttributeSet attributes;

    Presto::size_t material;

    Presto::size_t vertex_count;
    IndexList indices;

    // TODO: Make this a Presto type that can map to multiple graphics APIs
    std::int32_t draw_mode;
};

struct ImportedModel {
    PR_STRING_ID name;
    std::vector<ImportedMesh> meshes;
};

struct MaterialProperty {
    Presto::size_t binding;

    UniformVariableType type;
    PR_STRING_ID name;

    ErasedBytes data;

    ByteArray data{ByteArray(4, std::byte{0})};

    template <typename T>
    void setData(T newData) {
        auto new_size{sizeof(newData)};

        data.resize(new_size);
        std::memcpy(data.data(), newData, new_size);
    }

    template <typename S>
        requires std::ranges::range<S>
    void setData(S newData) {
        auto new_size{sizeof(newData.first()) * newData.size()};

        std::memcpy(data.data(), newData.data(), new_size);
    };
};

enum class MaterialType {
    DEFAULT_3D,
};

struct MaterialStructure {
    renderer_pipeline_id_t materialType{PR_PIPELINE_DEFAULT_3D};
    std::vector<MaterialProperty> properties;
};

struct ImportedMaterial {
    const MaterialStructure& structure;

    std::vector<MaterialProperty> properties;
};

const MaterialStructure GLTFMaterialStructure{
    .materialType = PR_PIPELINE_DEFAULT_3D,
    .properties = {
        {.binding = 0,
         .type = UniformVariableType::FLOAT,
         .name = "baseColour"},
        {.binding = 1, .type = UniformVariableType::IMAGE, .name = "diffuse"},
    }};

struct ImportedModelData {
    std::vector<ImportedModel> models;

    std::vector<ImportedMaterial> materials;
    std::vector<Presto::Image> images;
};

}  // namespace Presto
