#pragma once

#include <vulkan/vulkan.h>

#include <array>

#include <glm/glm.hpp>

namespace Presto {
    struct VulkanVertex {
        glm::vec3 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2>
        getAttributeDescriptions();
    };

    typedef std::vector<VulkanVertex> VertexList;
}  // namespace Presto
