#pragma once

#include "Types.h"

namespace Presto {

using PR_NUMERIC_ID = std::uint32_t;
using PR_SIZE = std::size_t;

enum RENDER_LIBRARY { UNSET, VULKAN, OPENGL, DIRECTX };

using Presto::PR_NUMERIC_ID;

enum PR_RESULT {
    PR_SUCCESS = 0,
    PR_FAILURE = 1,
};

using PR_BIT_TYPE = std::uint16_t;

constexpr PR_BIT_TYPE AS_BIT(std::uint8_t x) { return 1U << x; }

constexpr double PR_MINIMUM_NEAR_DISTANCE{0.1};

// Pipelines
using pipeline_id_t = Presto::uint16_t;
constexpr pipeline_id_t PR_PIPELINE_DEFAULT_3D = 0;
constexpr pipeline_id_t PR_PIPELINE_DEFAULT_UI = 1;

// Textures

using texture_id_t = Presto::uint16_t;
constexpr texture_id_t PR_DEFAULT_TEXTURE = 9;

constexpr PR_STRING_ID PR_ANY_NAME{"PRESTO_ANY_NAME"};
}  // namespace Presto
