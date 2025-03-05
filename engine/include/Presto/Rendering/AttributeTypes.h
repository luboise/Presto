#pragma once

#include "Presto/Types/CoreTypeTraits.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

using attribute_size_t = Presto::size_t;

struct BaseAttributeTypeDetails {
    Presto::size_t subtype_size;
    Presto::size_t count;
    Presto::size_t size;
};

template <typename T>
// TODO: Fix this constraint
// requires requires { SubTypeDetails<T>::subtype; }
struct AttributeTypeDetails : BaseAttributeTypeDetails {
    using details = SubTypeDetails<T>;
    using subtype = details::subtype;

    static constexpr Presto::size_t subtype_size = sizeof(subtype);
    static constexpr Presto::size_t count{details::subtype_count};
    static constexpr Presto::size_t size = subtype_size * count;
};

/*
template <ShaderDataType A>
struct ShaderDataTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::INT> {
    using ImportType = Presto::int32_t;
    using SubType = attributeTypeDetailsOf(ShaderDataType::INT)::sub_type;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::SHORT> {
    // TODO: Implement more specific type traits if needed
    // ShaderDataSubType sub_type{ShaderDataSubType::SHORT};
    // Presto::size_t sub_type_size{shaderDataSubTypeSize(sub_type)};
    // Presto::size_t sub_type_count{1};
    // Presto::size_t size{sub_type_size * sub_type_count};

    using ImportType = Presto::int32_t;
    using GPUType = ImportType;
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::FLOAT> {
    using ImportType = Presto::float32_t;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DOUBLE> {
    using ImportType = Presto::float64_t;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::UINT> {
    using ImportType = Presto::uint32_t;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC2> {
    using ImportType = Presto::vec2;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC3> {
    using ImportType = Presto::vec3;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC4> {
    using ImportType = Presto::vec4;
    using GPUType = ImportType;
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT4> {
    using ImportType = Presto::mat4;
    using GPUType = ImportType;
};

template <ShaderDataType T>
// Enforce specialisation
    requires requires { typename ShaderDataTypeTraits<T>; }
using ImportTypeOfShaderData = ShaderDataTypeTraits<T>::ImportType;
*/

////////
////////
////////

/*
template <ShaderDataType T>
struct AttributeTypeDetails {
    ShaderDataSubType sub_type;
    Presto::size_t sub_type_size;
    Presto::size_t sub_type_count;

    [[nodiscard]] Presto::size_t size() const {
        return sub_type_size * sub_type_count;
    }
};
*/

/*
template <>
struct AttributeTypeDetails<ShaderDataType::SHORT> {};

constexpr AttributeTypeDetails attributeTypeDetailsOf(
    ShaderDataType type) noexcept {
    switch (type) {
        case ShaderDataType::SHORT:
            return {.sub_type = ShaderDataSubType::SHORT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::SHORT),
                    .sub_type_count = 1};
        case ShaderDataType::USHORT:
            return {.sub_type = ShaderDataSubType::USHORT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::USHORT),
                    .sub_type_count = 1};
        case ShaderDataType::INT:
            return {
                .sub_type = ShaderDataSubType::INT,
                .sub_type_size = shaderDataSubTypeSize(ShaderDataSubType::INT),
                .sub_type_count = 1};
        case ShaderDataType::UINT:
            return {
                .sub_type = ShaderDataSubType::UINT,
                .sub_type_size = shaderDataSubTypeSize(ShaderDataSubType::INT),
                .sub_type_count = 1};
        case ShaderDataType::FLOAT:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 1};
        case ShaderDataType::DOUBLE:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 1};

        case ShaderDataType::VEC2:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 2};
        case ShaderDataType::DVEC2:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 2};

        case ShaderDataType::VEC3:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 3};
        case ShaderDataType::DVEC3:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 3};

        case ShaderDataType::VEC4:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 4};
        case ShaderDataType::DVEC4:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 4};

        case ShaderDataType::MAT3:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 12};
        case ShaderDataType::DMAT3:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 12};

        case ShaderDataType::MAT4:
            return {.sub_type = ShaderDataSubType::FLOAT,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::FLOAT),
                    .sub_type_count = 16};
        case ShaderDataType::DMAT4:
            return {.sub_type = ShaderDataSubType::DOUBLE,
                    .sub_type_size =
                        shaderDataSubTypeSize(ShaderDataSubType::DOUBLE),
                    .sub_type_count = 16};
    }
}
*/

}  // namespace Presto
