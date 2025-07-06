export module presto.types.core:traits;
import :base;

export namespace Pr {

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

}  // namespace Pr
