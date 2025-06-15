#pragma once

#include "Presto/Types/AssetTypes.h"

namespace Presto {

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
        vec4 colour{1, 1, 1, 1};
        ImagePtr diffuse_texture{nullptr};

        float32_t metallic{0};
        float32_t rough{0};

        ImagePtr normal_map{nullptr};
        ImagePtr bump_map{nullptr};

        float32_t specular;
        vec3 specular_tint{1, 1, 1};

        ImagePtr emission_map{nullptr};
    };
};

}  // namespace Presto
