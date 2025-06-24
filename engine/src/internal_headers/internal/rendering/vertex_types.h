#pragma once

#include "presto/core/logging.h"
#include "presto/core/type_traits.h"
#include "presto/core/types.h"

#include "presto/assets/material_types.h"

#include "vertex_templates.h"

namespace Pr {

struct BaseAttributeTypeDetails {
    Pr::size_t subtype_size;
    Pr::size_t count;
    Pr::size_t size;
};

template <typename T>
// TODO: Fix this constraint
// requires requires { SubTypeDetails<T>::subtype; }
struct AttributeTypeDetails : BaseAttributeTypeDetails {
    using details = SubTypeDetails<T>;
    using subtype = details::subtype;

    static constexpr Pr::size_t subtype_size = sizeof(subtype);
    static constexpr Pr::size_t count{details::subtype_count};
    static constexpr Pr::size_t size = subtype_size * count;
};

template <ShaderDataType T>
// requires requires { ShaderDataTypeTraits<T>::ImportType; }
using ShaderImportTypeOf = ShaderDataTypeTraits<T>::ImportType;

template <UniformVariableType T>
// Enforce specialisation
    requires requires { typename UniformVariableTypeTraits<T>; }
using ImportTypeOf = UniformVariableTypeTraits<T>::ImportType;

template <UniformVariableType T>
    requires requires { typename UniformVariableTypeTraits<T>; }
using GPUTypeOf = UniformVariableTypeTraits<T>::GPUType;

constexpr Pr::size_t SizeOfShaderType(ShaderDataType type);

BaseAttributeTypeDetails getShaderTypeDetails(ShaderDataType type);

template <>
struct UniformVariableTypeTraits<UniformVariableType::FLOAT> {
    using ImportType = Pr::float32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::INT> {
    using ImportType = Pr::int32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::UINT> {
    using ImportType = Pr::uint32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC2> {
    using ImportType = Pr::vec2;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC3> {
    using ImportType = Pr::vec3;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC4> {
    using ImportType = Pr::vec4;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::TEXTURE> {
    // Imported as index into the array of imported textures
    using ImportType = Pr::uint8_t;
    using GPUType = Pr::int32_t;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::MAT4> {
    using ImportType = Pr::mat4;
    using GPUType = ImportType;
};

}  // namespace Pr
