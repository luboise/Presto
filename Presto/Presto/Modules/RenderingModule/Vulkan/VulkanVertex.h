#pragma once

#include "../Types/DrawInfo.h"

#include <vulkan/vulkan.h>

#include <array>

#include <glm/glm.hpp>

namespace Presto {
#define index_t uint16_t

    struct VulkanVertex {
        glm::vec3 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2>
        getAttributeDescriptions();
    };

    typedef std::vector<VulkanVertex> VertexList;
}  // namespace Presto
