#pragma once

namespace Presto {

enum class UniformVariableType {
    INT,
    UINT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT4,
    IMAGE,
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
struct UniformVariableTypeTraits<UniformVariableType::IMAGE> {
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

Presto::size_t SizeOfType(UniformVariableType type) noexcept {
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
        SWITCH_CASE(UniformVariableType::IMAGE);
    }
#undef SWITCH_CASE
};

}  // namespace Presto
