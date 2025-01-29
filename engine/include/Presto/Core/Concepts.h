#pragma once

namespace Presto {
    template <typename Derived, typename Base>
    concept DerivedFrom =
        std::is_base_of_v<Base, Derived> &&
        std::is_convertible_v<const volatile Derived*, const volatile Base*>;

    using Scalar = float;

    template <typename T>
    concept Scalable = requires(T a, Scalar b) {
        { a* b } -> std::convertible_to<T>;
        { a / b } -> std::convertible_to<T>;
    };
}  // namespace Presto
