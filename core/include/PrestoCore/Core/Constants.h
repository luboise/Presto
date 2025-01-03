#pragma once

#include <cstdint>

namespace Presto {
    enum RENDER_LIBRARY { UNSET, VULKAN, OPENGL, DIRECTX };
}

enum PR_RESULT {
    PR_SUCCESS = 0,
    PR_FAILURE = 1,
};

using id_t = uint32_t;
