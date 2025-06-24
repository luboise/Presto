#pragma once

#include "presto/assets/material_types.h"
#include "presto/core/types.h"

namespace Pr {

template <ShaderDataType T>
struct ShaderDataTypeTraits {
    static_assert(false, "No shader data type details struct implemented.");
};

template <UniformVariableType T>
struct UniformVariableTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::SHORT> {
    using ImportType = Pr::int16_t;
    static constexpr Pr::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::USHORT> {
    using ImportType = Pr::uint16_t;
    static constexpr Pr::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::INT> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::UINT> {
    using ImportType = Pr::uint16_t;
    static constexpr Pr::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::FLOAT> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DOUBLE> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{1};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC2> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{2};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC2> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{2};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC3> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{3};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC3> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{3};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC4> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{4};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC4> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{4};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT3> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{12};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT3> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{12};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT4> {
    using ImportType = Pr::float32_t;
    static constexpr Pr::size_t subtype_count{16};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT4> {
    using ImportType = Pr::float64_t;
    static constexpr Pr::size_t subtype_count{16};
};

}  // namespace Pr
