#pragma once

#include <vulkan/vulkan_core.h>
#include <map>
#include "Presto/Components/Renderable.h"

namespace Presto {
    struct ShaderMatrices {
        glm::mat4 view;
        glm::mat4 projection;
    };

    struct DrawInfo {
        size_t vertex_count = 0;

        // Vulkan uses signed int for the offset
        int32_t vertex_offset = 0;

        size_t index_count = 0;
        uint32_t index_offset = 0;
    };

    using draw_info_key = Renderable*;

    template <typename T>
    using DrawInfoMap = std::map<draw_info_key, T>;
}  // namespace Presto
