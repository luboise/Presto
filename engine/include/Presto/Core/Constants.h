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

}  // namespace Presto
