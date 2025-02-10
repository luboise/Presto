#pragma once

#include "Presto/Assets/ImageAsset.h"

namespace Presto {

struct PBRMaterial {};

enum class MaterialType : std::uint8_t { DEFAULT_3D, DEFAULT_UI, CUSTOM };

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

}  // namespace Presto
