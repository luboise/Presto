#pragma once

#include <vulkan/vulkan.h>

#include <array>
#include <glm/glm.hpp>

namespace Presto {
    struct VulkanVertex {
        glm::vec3 pos;
        glm::vec3 color;

        glm::vec3 getProjected(glm::vec3 yawPitchRoll) const;
        glm::vec3 getProjected(glm::vec3 yawPitchRoll,
                               glm::vec3 cameraPos) const;
        glm::vec3 getProjected(glm::vec3 yawPitchRoll, glm::vec3 cameraPos,
                               glm::float32 scale) const;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2>
        getAttributeDescriptions();

        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::vec3 yawPitchRoll,
                                            glm::float32 scale);
        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::float32 scale);
        static glm::mat4 getProjectionMatrix(glm::float32 fovRad,
                                             VkExtent2D extents);
    };
}  // namespace Presto
