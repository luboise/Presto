#pragma once

namespace Presto {
    using CheckedComponentBits = uint32_t;
    using CheckedComponentBit = CheckedComponentBits;

    constexpr CheckedComponentBit TRANSFORM_BIT = AS_BIT(0);
    constexpr CheckedComponentBit MODEL_BIT = AS_BIT(1);
    constexpr CheckedComponentBit CAMERA_BIT = AS_BIT(2);
}  // namespace Presto
