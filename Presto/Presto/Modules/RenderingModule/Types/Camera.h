#include <glm/glm.hpp>

namespace Presto {
    class PRESTO_API Camera {
       public:
        Camera();

        glm::mat4 getViewMatrix() const;

        void setYaw(double newYaw);
        void setPitch(double newPitch);
        void setRoll(double newRoll);
        void setPos(glm::vec3 newPos);

        double getYaw() const;
        double getPitch() const;
        double getRoll() const;
        glm::vec3 getPos() const;

       private:
        glm::vec3 _cameraPos = glm::vec3(0, 0, 0.75);
        double _yaw = 0.0;
        double _pitch = 0.0;
        double _roll = 0.0;

        void recalculate();

        glm::highp_mat4 _viewMatrix;
    };
}  // namespace Presto