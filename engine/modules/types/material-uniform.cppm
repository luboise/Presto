export module presto.types.material:uniform;

import presto.types.core;

export namespace Pr {

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

template <UniformVariableType T>
struct UniformVariableTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

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

template <UniformVariableType T>
// Enforce specialisation
    requires requires { typename UniformVariableTypeTraits<T>; }
using ImportTypeOf = UniformVariableTypeTraits<T>::ImportType;

template <UniformVariableType T>
    requires requires { typename UniformVariableTypeTraits<T>; }
using GPUTypeOf = UniformVariableTypeTraits<T>::GPUType;

constexpr Pr::size_t SizeOfType(UniformVariableType type) noexcept {
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

}  // namespace Pr
