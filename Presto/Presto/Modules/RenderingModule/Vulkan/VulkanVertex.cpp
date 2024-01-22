#include "VulkanVertex.h"

namespace Presto {
    VkVertexInputBindingDescription VulkanVertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};

        // Binding 0 in array of bindings
        bindingDescription.binding = 0;
        // How far to skip for each read (want to skip size of each vertex)
        bindingDescription.stride = sizeof(VulkanVertex);
        // Go to next vertex after this instance, or after processing this
        // vertex for all instances
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    std::array<VkVertexInputAttributeDescription, 2>
    VulkanVertex::getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2>
            attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

        return attributeDescriptions;
    }
}  // namespace Presto