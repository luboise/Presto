#pragma once

namespace Presto {
enum class Strictness { STRICT, LOOSE };
using Strictness::LOOSE;
using Strictness::STRICT;

template <typename Derived, typename Base,
          Strictness Strict = Strictness::LOOSE>
concept DerivedFrom = std::is_base_of_v<Base, Derived> && requires {
    (Strict == Strictness::STRICT)
        ?
        // Perform this check only if the strictness is STRICT
        (requires {
            !std::is_same_v<Base, Derived>;
            requires std::is_convertible_v<const volatile Derived*,
                                           const volatile Base*>;
        })
        : (requires {
              std::is_convertible_v<const volatile Derived*,
                                    const volatile Base*>;
          });
};

using Scalar = float;

template <typename T>
concept Scalable = requires(T a, Scalar b) {
    { a* b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
};
}  // namespace Presto
