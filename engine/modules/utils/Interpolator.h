#pragma once

#include "Presto/Core/Concepts.h"

namespace Presto {
template <Scalable T>
class PRESTO_API Interpolator {
   public:
    explicit Interpolator() = default;
    explicit Interpolator(T val) : Interpolator() { setValue(val); };

    void setValue(T newDiff) { value_ = newDiff; }

    T getRatio(Scalar ratio) {
        ratio = std::clamp(ratio, 0.0F, 1.0F);
        return value_ * ratio;
    };

   private:
    T value_;
};
}  // namespace Presto
