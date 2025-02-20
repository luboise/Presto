#pragma once

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Rendering/ErasedBytes.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

struct PBRMaterial {};

enum class MaterialType : std::uint8_t {
    DEFAULT_3D,
    DEFAULT_UI,
    /*SHADER*/
};  // FEATURE: Implement shader materials

template <MaterialType T>
struct MaterialTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

template <>
struct MaterialTypeTraits<MaterialType::DEFAULT_3D> {
    struct property_list {
        Presto::vec4 colour{1, 1, 1, 1};
        Presto::ImagePtr diffuse_texture{nullptr};

        Presto::float32_t metallic{0};
        Presto::float32_t rough{0};

        Presto::ImagePtr normal_map{nullptr};
        Presto::ImagePtr bump_map{nullptr};

        Presto::float32_t specular;
        Presto::vec3 specular_tint{1, 1, 1};

        Presto::ImagePtr emission_map{nullptr};
    };
};

struct MaterialStructure {
    std::vector<UniformBinding> bindings;
    std::vector<UniformBlock> blocks;
};

struct MaterialProperty {
    PR_STRING_ID name;
    UniformVariableType type;

    Presto::size_t binding;
    Presto::uint8_t offset;

    static bool compatible(const MaterialProperty&, const MaterialProperty&);
};

struct MaterialBlockProperty;

struct PipelineStructure;

/*
struct MaterialStructure {
    std::vector<MaterialProperty> properties;
    ErasedBytes data;

    [[nodiscard]] const MaterialProperty* getProperty(
        const PR_STRING_ID& name) const;

    [[nodiscard]] bool writeableFrom(const MaterialStructure&) const;

    void merge(const MaterialStructure&);

    static MaterialStructure from(const PipelineStructure&);

    void writeFrom(const MaterialProperty& other);

[[nodiscard]] bool compatibleWith(const MaterialProperty& other) const {
    return MaterialProperty::compatible(*this, other);
};

};
*/

}  // namespace Presto
