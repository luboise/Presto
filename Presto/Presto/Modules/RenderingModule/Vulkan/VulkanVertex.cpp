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

    glm::vec3 VulkanVertex::getProjected(glm::vec3 yawPitchRoll) const {
        return getProjected(yawPitchRoll, glm::vec3(0, 0, 10.0f));
    }

    glm::vec3 VulkanVertex::getProjected(glm::vec3 yawPitchRoll,
                                         glm::vec3 cameraPos) const {
        return getProjected(yawPitchRoll, cameraPos, 1.0);
    }

    glm::vec3 VulkanVertex::getProjected(glm::vec3 yawPitchRoll,
                                         glm::vec3 cameraPos,
                                         glm::float32 scale) const {
        auto fovYDeg = 90;
        VkExtent2D viewport{1280, 720};

        auto mvm = getModelViewMatrix(cameraPos, yawPitchRoll, scale);
        auto pm = getProjectionMatrix(fovYDeg, viewport);

        auto projected = pm * mvm * glm::vec4(this->pos, 1);

        // Normalise to -1, 1
        auto normalised = glm::vec3(projected) / projected.w;
        return normalised;
    }

    glm::mat4 VulkanVertex::getModelViewMatrix(glm::vec3 offset,
                                               glm::float32 scale) {
        return getModelViewMatrix(offset, glm::vec3(0.0f), scale);
    }

    glm::mat4 VulkanVertex::getModelViewMatrix(glm::vec3 offset,
                                               glm::vec3 yawPitchRoll,
                                               glm::float32 scale) {
        glm::mat4 model(1.0f);

        model = glm::rotate(model, yawPitchRoll.y, glm::vec3(1, 0, 0));
        model = glm::rotate(model, yawPitchRoll.x, glm::vec3(0, 1, 0));
        model = glm::rotate(model, yawPitchRoll.z, glm::vec3(0, 0, 1));

        glm::mat4 scaleMatrix = {
            {scale, 0, 0, 0},
            {0, scale, 0, 0},
            {0, 0, scale, 0},
            {0, 0, 0, 1},
        };
        model = scaleMatrix * model;

        // Calculate offset pointing at origin with the y axis up
        glm::mat4 view =
            glm::lookAt(offset, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        return view * model;
    }

    glm::mat4 VulkanVertex::getProjectionMatrix(glm::float32 fovDeg,
                                                VkExtent2D extents) {
        float fovRad = glm::radians(fovDeg);
        glm::mat4 projection =
            glm::perspectiveFov(fovRad, (glm::float32)extents.width,
                                (glm::float32)extents.height, 0.01f, 100.0f);

        return projection;
    }
}  // namespace Presto