#include <vector>

#include "presto/core/types.h"

namespace Presto {

using uniform_name_t = Presto::string;

using uniform_index_t = Presto::uint8_t;
constexpr uniform_index_t PR_INVALID_UNIFORM = -1;

// Uniform variable types
enum class UniformVariableType : Presto::uint8_t {
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
    enum : Presto::uint8_t { SINGLE, BLOCK };

    Presto::uint8_t bind_type;
    UniformVariableType data_type;
    uniform_name_t name;

    union {
        Presto::uint32_t location;
        Presto::uint32_t offset;
    };

    [[nodiscard]] Presto::size_t size() const;
};

struct UniformBlock {
    uniform_index_t bind_point;
    uniform_name_t name;

    std::vector<UniformBinding> bindings;

    [[nodiscard]] Presto::size_t size() const;
};

struct UniformLayout {
    std::vector<UniformBinding> bindings;
    std::vector<UniformBlock> blocks;
};

struct MaterialProperty {
    Presto::string name;
    UniformVariableType type;

    Presto::size_t binding;
    Presto::uint8_t offset;

    static bool compatible(const MaterialProperty&, const MaterialProperty&);
};

}  // namespace Presto
