#include "VulkanVertex.h"

namespace Presto {
constexpr glm::float32 cameraZDistance = 2.0F;

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
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VulkanVertex, colour);

    // attributeDescriptions[2].binding = 0;
    // attributeDescriptions[2].location = 2;
    // attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    // attributeDescriptions[2].offset = offsetof(VulkanVertex,
    // modelMatrix);

    // attributeDescriptions[3].binding = 0;
    // attributeDescriptions[3].location = 3;
    // attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    // attributeDescriptions[3].offset = offsetof(VulkanVertex,
    // modelMatrix);

    return attributeDescriptions;
}

}  // namespace Presto
