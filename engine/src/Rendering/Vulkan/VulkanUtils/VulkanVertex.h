#pragma once

#include "Presto/Rendering/Vertex.h"

#include <vulkan/vulkan.h>

#include <array>

namespace Presto {
#define index_t uint16_t

struct VulkanVertex : public Vertex {
    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>

    getAttributeDescriptions();
};

}  // namespace Presto
