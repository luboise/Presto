#include "types.h"

namespace Pr {

constexpr Pr::size_t shaderDataSubTypeSize(ShaderDataSubType subType) noexcept {
    switch (subType) {
        case ShaderDataSubType::SHORT:
            return sizeof(Pr::int16_t);
        case ShaderDataSubType::USHORT:
            return sizeof(Pr::uint16_t);
        case ShaderDataSubType::INT:
            return sizeof(Pr::int32_t);
        case ShaderDataSubType::UINT:
            return sizeof(Pr::uint32_t);
        case ShaderDataSubType::FLOAT:
            return sizeof(Pr::float32_t);
        case ShaderDataSubType::DOUBLE:
            return sizeof(Pr::float64_t);
    }
};

// If no specialisation specified, then T is a subtype of itself
template <typename T>
struct SubTypeDetails {
    using subtype = T;
};

template <>
struct SubTypeDetails<Pr::uint16_t> {
    using subtype = Pr::uint16_t;
};

template <>
struct SubTypeDetails<Pr::int16_t> {
    using subtype = Pr::int16_t;
};

template <>
struct SubTypeDetails<Pr::vec3> {
    using subtype = Pr::float32_t;
};

template <>
struct SubTypeDetails<Pr::vec4> {
    using subtype = Pr::float32_t;
};

template <>
struct SubTypeDetails<Pr::float32_t> {
    using subtype = Pr::float32_t;
};

template <typename T>
// requires requires { SubTypeDetails<T>::type; }
using SubTypeOf = SubTypeDetails<T>::subtype;

constexpr ShaderDataSubType getShaderDataSubType(ShaderDataType data) {
    switch (data) {
        case ShaderDataType::SHORT:
            return ShaderDataSubType::SHORT;
        case ShaderDataType::USHORT:
            return ShaderDataSubType::USHORT;
        case ShaderDataType::INT:
            return ShaderDataSubType::INT;
        case ShaderDataType::UINT:
            return ShaderDataSubType::UINT;
        case ShaderDataType::FLOAT:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DOUBLE:
            return ShaderDataSubType::DOUBLE;
        case ShaderDataType::VEC2:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DVEC2:
            return ShaderDataSubType::DOUBLE;
        case ShaderDataType::VEC3:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DVEC3:
            return ShaderDataSubType::DOUBLE;
        case ShaderDataType::VEC4:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DVEC4:
            return ShaderDataSubType::DOUBLE;
        case ShaderDataType::MAT3:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DMAT3:
            return ShaderDataSubType::DOUBLE;
        case ShaderDataType::MAT4:
            return ShaderDataSubType::FLOAT;
        case ShaderDataType::DMAT4:
            return ShaderDataSubType::DOUBLE;
    }
};
}  // namespace Pr
