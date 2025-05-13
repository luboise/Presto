#pragma once

namespace Presto {
enum class Strictness { STRICTLY_DERIVED, LOOSELY_DERIVED };
using Strictness::LOOSELY_DERIVED;
using Strictness::STRICTLY_DERIVED;

template <typename Derived, typename Base,
          Strictness Strict = Strictness::LOOSELY_DERIVED>
concept DerivedFrom = std::is_base_of_v<Base, Derived> && requires {
    (Strict == Strictness::STRICTLY_DERIVED)
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
