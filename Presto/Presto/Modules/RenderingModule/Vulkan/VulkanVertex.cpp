#include "VulkanVertex.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Presto {
    const glm::float32 cameraZDistance = 2.0f;

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
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

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