#pragma once

#include "PrestoCore/Rendering/Vertex.h"

#include <vulkan/vulkan.h>

#include <array>

namespace PrestoCore {
#define index_t uint16_t

    struct VulkanVertex : public Vertex {
        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2>

        getAttributeDescriptions();
    };

}  // namespace PrestoCore
