#include "presto/assets/types.h"
#include "presto/core/types.h"

namespace Presto {

enum class MaterialType : Presto::uint8_t {
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

}  // namespace Presto
