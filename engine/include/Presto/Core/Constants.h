#pragma once

#include <cstdint>

namespace Presto {
    using PR_NUMERIC_ID = uint32_t;

    enum RENDER_LIBRARY { UNSET, VULKAN, OPENGL, DIRECTX };

    using Presto::PR_NUMERIC_ID;

    enum PR_RESULT {
        PR_SUCCESS = 0,
        PR_FAILURE = 1,
    };

    using PR_BIT_TYPE = std::uint16_t;

    constexpr PR_BIT_TYPE AS_BIT(uint8_t x) { return 1U << x; }
}  // namespace Presto
