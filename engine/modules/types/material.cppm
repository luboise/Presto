export module presto.types.material;

export import :uniform;
export import :shader;

import std;

import presto.types.core;

export namespace Pr {

using uniform_name_t = Pr::string;

using uniform_index_t = Pr::uint8_t;
constexpr uniform_index_t PR_INVALID_UNIFORM = -1;

struct UniformBinding {
    enum : Pr::uint8_t { SINGLE, BLOCK };

    Pr::uint8_t bind_type;
    UniformVariableType data_type;
    uniform_name_t name;

    union {
        Pr::uint32_t location;
        Pr::uint32_t offset;
    };

    [[nodiscard]] Pr::size_t size() const;
};

struct UniformBlock {
    uniform_index_t bind_point;
    uniform_name_t name;

    std::vector<UniformBinding> bindings;

    [[nodiscard]] Pr::size_t size() const;
};

struct UniformLayout {
    std::vector<UniformBinding> bindings;
    std::vector<UniformBlock> blocks;
};

struct MaterialProperty {
    Pr::string name;
    UniformVariableType type;

    Pr::size_t binding;
    Pr::uint8_t offset;

    static bool compatible(const MaterialProperty&, const MaterialProperty&);
};

enum class MaterialType : Pr::uint8_t {
    DEFAULT_3D,
    DEFAULT_UI,
    /*SHADER*/
};  // FEATURE: Implement shader materials

/*
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
*/

}  // namespace Pr
