export module presto.assets.material.types;

import std;

import presto.core.types;

export namespace Pr {

using uniform_name_t = Pr::string;

using uniform_index_t = Pr::uint8_t;
constexpr uniform_index_t PR_INVALID_UNIFORM = -1;

// Uniform variable types
enum class UniformVariableType : Pr::uint8_t {
    INT = 1,
    UINT = 2,
    FLOAT = 3,
    VEC2 = 4,
    VEC3 = 5,
    VEC4 = 6,
    MAT3 = 7,
    MAT4 = 8,
    TEXTURE = 9,
};

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

}  // namespace Pr
