#pragma once

#include "Presto/Rendering/AttributeTypes.h"

namespace Presto {

template <ShaderDataType T>
struct ShaderDataTypeTraits {
    static_assert(false, "No shader data type details struct implemented.");
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::SHORT> {
    using ImportType = Presto::int16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::USHORT> {
    using ImportType = Presto::uint16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::INT> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::UINT> {
    using ImportType = Presto::uint16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::FLOAT> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DOUBLE> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{1};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC2> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{2};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC2> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{2};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC3> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{3};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC3> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{3};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC4> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{4};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC4> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{4};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT3> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{12};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT3> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{12};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT4> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{16};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT4> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{16};
};

template <ShaderDataType T>
// requires requires { ShaderDataTypeTraits<T>::ImportType; }
using ShaderImportTypeOf = ShaderDataTypeTraits<T>::ImportType;

constexpr Presto::size_t ShaderTypeSize(ShaderDataType type) {
#define SWITCH_CASE(type)                                         \
    case type:                                                    \
        return sizeof(ShaderDataTypeTraits<(type)>::ImportType) * \
               ShaderDataTypeTraits<(type)>::subtype_count;

    switch (type) {
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);

        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC3);

        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::MAT3);

        SWITCH_CASE(ShaderDataType::MAT4);

        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::DMAT4);

        default: {
            PR_ERROR("No size available.");
            return 0;
        }
    }
#undef SWITCH_CASE
}

BaseAttributeTypeDetails getShaderTypeDetails(ShaderDataType type);

}  // namespace Presto
