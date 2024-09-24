#pragma once

#include <vulkan/vulkan_core.h>
namespace Presto {
    struct ShaderMatrices {
        glm::mat4 modelView;
        glm::mat4 projection;
    };

    struct DrawInfo {
        size_t vertex_count = 0;

        // Vulkan uses signed int for the offset
        int32_t vertex_offset = 0;

        size_t index_count = 0;
        uint32_t index_offset = 0;
    };

}  // namespace Presto
