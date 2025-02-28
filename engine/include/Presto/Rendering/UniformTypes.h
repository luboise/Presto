#pragma once

namespace Presto {

enum class UniformVariableType : uint8_t {
    INT,
    UINT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT4,
    TEXTURE,
};

template <UniformVariableType T>
struct UniformVariableTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

template <>
struct UniformVariableTypeTraits<UniformVariableType::FLOAT> {
    using ImportType = Presto::float32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::INT> {
    using ImportType = Presto::int32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::UINT> {
    using ImportType = Presto::uint32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC2> {
    using ImportType = Presto::vec2;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC3> {
    using ImportType = Presto::vec3;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC4> {
    using ImportType = Presto::vec4;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::TEXTURE> {
    // Imported as index into the array of imported textures
    using ImportType = Presto::uint8_t;
    using GPUType = Presto::int32_t;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::MAT4> {
    using ImportType = Presto::mat4;
    using GPUType = ImportType;
};

template <UniformVariableType T>
// Enforce specialisation
    requires requires { typename UniformVariableTypeTraits<T>; }
using ImportTypeOf = UniformVariableTypeTraits<T>::ImportType;

template <UniformVariableType T>
    requires requires { typename UniformVariableTypeTraits<T>; }
using GPUTypeOf = UniformVariableTypeTraits<T>::GPUType;

constexpr Presto::size_t SizeOfType(UniformVariableType type) noexcept {
#define SWITCH_CASE(type) \
    case type:            \
        return sizeof(UniformVariableTypeTraits<type>::GPUType);

    switch (type) {
        SWITCH_CASE(UniformVariableType::INT);
        SWITCH_CASE(UniformVariableType::UINT);
        SWITCH_CASE(UniformVariableType::FLOAT);
        SWITCH_CASE(UniformVariableType::VEC2);
        SWITCH_CASE(UniformVariableType::VEC3);
        SWITCH_CASE(UniformVariableType::VEC4);
        SWITCH_CASE(UniformVariableType::MAT4);
        SWITCH_CASE(UniformVariableType::TEXTURE);
        default: {
            return 0;
        }
    }
#undef SWITCH_CASE
};

using uniform_name_t = PR_STRING_ID;

using uniform_index_t = Presto::uint8_t;
constexpr uniform_index_t PR_INVALID_UNIFORM = -1;

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

}  // namespace Presto
