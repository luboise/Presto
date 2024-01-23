#include "VulkanVertex.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

        return attributeDescriptions;
    }

    glm::vec3 VulkanVertex::getProjected(glm::vec3 yawPitchRoll) const {
        auto fov = glm::pi<glm::float32>() / 180 * 90;
        VkExtent2D viewport{1280, 720};

        auto scale = 0.5;
        auto mvm = getModelViewMatrix(this->pos, yawPitchRoll, scale);
        auto pm = getProjectionMatrix(fov, viewport);

        auto val =
            // glm::project(this->pos, mvm, pm,
            // glm::vec4(0, 0, viewport.width, viewport.height));
            pm * mvm * glm::vec4(this->pos, 1);
        return glm::vec3(val);
    }

    glm::mat4 VulkanVertex::getModelViewMatrix(glm::vec3 offset,
                                               glm::float32 scale) {
        return getModelViewMatrix(offset, glm::vec3(0.0f), scale);
    }

    glm::mat4 VulkanVertex::getModelViewMatrix(glm::vec3 offset,
                                               glm::vec3 yawPitchRoll,
                                               glm::float32 scale) {
        glm::float32 cameraNear = -2.0f;

        glm::mat4 model(1.0f);

        model = glm::rotate(model, yawPitchRoll.x, glm::vec3(0, 1, 0));
        model = glm::rotate(model, yawPitchRoll.y, glm::vec3(1, 0, 0));
        model = glm::rotate(model, yawPitchRoll.z, glm::vec3(0, 0, 1));
        model = glm::translate(model, offset);
        glm::mat4 scaleMatrix = {
            {scale, 0, 0, 0},
            {0, scale, 0, 0},
            {0, 0, scale, 0},
            {0, 0, 0, 1},
        };
        model = scaleMatrix * model;

        glm::mat4 cameraModel(1.0f);
        cameraModel = glm::translate(cameraModel, glm::vec3(0, 0, cameraNear));
        // cameraModel = glm::rotateY(cameraModel,
        //                            glm::sin(glm::quarter_pi<glm::float32>()));
        glm::mat4 view = glm::inverse(cameraModel);

        return view * model;
    }

    glm::mat4 VulkanVertex::getProjectionMatrix(glm::float32 fovRad,
                                                VkExtent2D extents) {
        glm::mat4 projection =
            glm::perspectiveFov(fovRad, (glm::float32)extents.width,
                                (glm::float32)extents.height, -1.0f, 3.0f);

        return projection;
    }
}  // namespace Presto