export module presto.materials:types;

import presto.core.types;
import presto.assets.types;

export namespace Pr {

enum class MaterialType : Pr::uint8_t {
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
        Pr::vec4 colour{1, 1, 1, 1};
        Pr::Ptr<Pr::ImageAsset> diffuse_texture{nullptr};

        Pr::float32_t metallic{0};
        Pr::float32_t rough{0};

        Pr::Ptr<Pr::ImageAsset> normal_map{nullptr};
        Pr::Ptr<Pr::ImageAsset> bump_map{nullptr};

        Pr::float32_t specular;
        Pr::vec3 specular_tint{1, 1, 1};

        Pr::Ptr<Pr::ImageAsset> emission_map{nullptr};
    };
};

}  // namespace Pr
