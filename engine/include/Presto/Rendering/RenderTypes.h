#pragma once

#include <vulkan/vulkan_core.h>

#include "glm/ext/matrix_float4x4.hpp"

namespace Presto {
    struct ShaderMatrices {
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 transform;
    };

    struct DrawInfo {
        size_t vertex_count = 0;

        // Vulkan uses signed int for the offset
        int32_t vertex_offset = 0;

        size_t index_count = 0;
        uint32_t index_offset = 0;
    };
}  // namespace Presto
