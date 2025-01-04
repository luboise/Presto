#pragma once

#include <glm/glm.hpp>

namespace Presto {
    class PRESTO_API Camera {
       public:
        Camera();

        [[nodiscard]] glm::mat4 getViewMatrix() const;

        void setYaw(double newYaw);
        void setPitch(double newPitch);
        void setRoll(double newRoll);
        void setPos(glm::vec3 newPos);
        void setFocus(glm::vec3 newPos);

        [[nodiscard]] double getYaw() const;
        [[nodiscard]] double getPitch() const;
        [[nodiscard]] double getRoll() const;
        [[nodiscard]] glm::vec3 getPos() const;
        [[nodiscard]] glm::vec3 getFocus() const;

       private:
        glm::vec3 _cameraPos = glm::vec3(0, 0, 0.75);
        glm::vec3 _focusPoint = glm::vec3(0, 0, 0);
        double _yaw = 0.0;
        double _pitch = 0.0;
        double _roll = 0.0;

        void recalculate();

        glm::highp_mat4 _viewMatrix{1};
    };
}  // namespace Presto
