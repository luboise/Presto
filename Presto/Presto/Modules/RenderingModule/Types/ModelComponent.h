#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Presto {
    class ModelComponent {
       public:
        // glm::vec3 getProjected(glm::vec3 yawPitchRoll) const;
        // glm::vec3 getProjected(glm::vec3 yawPitchRoll,
        //                        glm::vec3 cameraPos) const;
        // glm::vec3 getProjected(glm::vec3 yawPitchRoll, glm::vec3 cameraPos,
        //                        glm::float32 scale) const;

        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::vec3 yawPitchRoll,
                                            glm::float32 scale);
        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::float32 scale);
        // static glm::mat4 getProjectionMatrix(glm::float32 fovRad,
        //                                      VkExtent2D extents);

        void setScale(glm::vec3 scale);
        void setTranslation(glm::vec3 translation);
        void setRotation(glm::vec3 yawPitchRoll);

        glm::vec3 getModelView() const;

        glm::vec3 getScale() const;
        glm::vec3 getTranslation() const;

        // Returns vec3(yaw,pitch,roll)
        glm::vec3 getRotation() const;

        glm::vec3 getYaw() const;
        glm::vec3 getPitch() const;
        glm::vec3 getRoll() const;

       private:
        glm::vec3 _scale;
        glm::vec3 _translation;
        glm::vec3 _yawPitchRoll;
        glm::mat4 _modelView;
    };
}  // namespace Presto