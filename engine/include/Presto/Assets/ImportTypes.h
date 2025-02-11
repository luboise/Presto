#pragma once

#include "Presto/Assets/Image.h"

#include "Presto/Rendering/AttributeTypes.h"

#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

struct ImportedMesh {
    struct ImportedAttribute {
        PR_STRING_ID name;

        ShaderDataType type;
        Presto::size_t count;

        ByteArray data;
    };

    std::vector<ImportedAttribute> attributes;

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

namespace DefaultAttributeName {
constexpr auto POSITION = "a_position";
constexpr auto NORMAL = "a_normal";
constexpr auto TEXCOORDS = "a_texcoords";

}  // namespace DefaultAttributeName

namespace DefaultMaterialPropertyName {
constexpr auto BASE_COLOUR = "u_baseColour";

// Alias for American english speakers
constexpr auto BASE_COLOR = BASE_COLOUR;

constexpr auto DIFFUSE_TEXTURE = "u_diffuseTexture";

};  // namespace DefaultMaterialPropertyName

struct MaterialProperty {
    Presto::size_t binding;
    UniformVariableType type;
    PR_STRING_ID name;

    ErasedBytes data;

    void writeFrom(const MaterialProperty& other);

    [[nodiscard]] bool compatibleWith(const MaterialProperty& other) const {
        return MaterialProperty::compatible(*this, other);
    };

    static bool compatible(const MaterialProperty&, const MaterialProperty&);
};

namespace DefaultMaterialProperties {

MaterialProperty BaseColour(Presto::vec4 colour) {
    return {.binding = 0,
            .type = UniformVariableType::VEC3,
            .name = DefaultMaterialPropertyName::BASE_COLOUR,
            .data = ErasedBytes{colour}};
}
MaterialProperty DiffuseTexture(
    ImportTypeOf<UniformVariableType::IMAGE> index) {
    return {.binding = 0,
            .type = UniformVariableType::IMAGE,
            .name = DefaultMaterialPropertyName::DIFFUSE_TEXTURE,
            .data = ErasedBytes{index}};
}

};  // namespace DefaultMaterialProperties

struct MaterialStructure {
    std::vector<MaterialProperty> properties;

    [[nodiscard]] const MaterialProperty* getProperty(
        const PR_STRING_ID& name) const;

    [[nodiscard]] bool writeableFrom(const MaterialStructure&) const;

    void merge(const MaterialStructure&);

    static MaterialStructure from(const PipelineStructure&);
};

struct PropertyList {
    std::vector<MaterialProperty> properties;
};

struct ImportedMaterial {
    PR_STRING_ID name;
    renderer_pipeline_id_t materialType{PR_PIPELINE_DEFAULT_3D};
    MaterialStructure structure;
};

// TODO: Make this a proper struct with other properties
using ImportedImage = Presto::Image;

struct ImportedModelData {
    std::vector<ImportedModel> models;

    std::vector<ImportedMaterial> materials;
    std::vector<ImportedImage> images;
};

}  // namespace Presto
