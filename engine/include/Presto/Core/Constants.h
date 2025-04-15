#pragma once

#include "Presto/Types/CoreTypes.h"

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

constexpr double PR_MIN_NEAR_DISTANCE{0.1};

// Pipelines
using mesh_context_id_t = Presto::uint32_t;

using mesh_registration_id_t = Presto::uint32_t;
constexpr mesh_registration_id_t PR_UNREGISTERED{
    static_cast<mesh_registration_id_t>(-1)};

using pipeline_id_t = Presto::uint16_t;
using material_id_t = Presto::uint16_t;

constexpr pipeline_id_t PR_MIN_USER_PIPELINE_ID{10};
constexpr mesh_registration_id_t PR_MIN_RUNTIME_MESH_ID{10};

constexpr pipeline_id_t PR_PIPELINE_ANY{0};
constexpr pipeline_id_t PR_PIPELINE_DEFAULT_3D{1};
constexpr pipeline_id_t PR_PIPELINE_DEFAULT_UI{2};
constexpr pipeline_id_t PR_PIPELINE_NONE{static_cast<pipeline_id_t>(-1)};

// Textures

using texture_id_t = Presto::uint16_t;
constexpr texture_id_t PR_DEFAULT_TEXTURE{9};

constexpr mesh_registration_id_t PR_MESH_QUAD{1};
constexpr mesh_registration_id_t PR_MESH_0_SQUARE{2};

constexpr PR_STRING_ID PR_ANY_NAME{"PRESTO_ANY_NAME"};

constexpr auto DEFAULT_FOV = 120;
}  // namespace Presto
