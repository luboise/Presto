#include "Presto/Objects/Component.h"

enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

namespace Presto {
    class Camera : public Component {
       public:
        Camera();

        [[nodiscard]] Presto::mat4 getViewMatrix() const;

        Camera& setYaw(double newYaw);
        Camera& setPitch(double newPitch);
        Camera& setRoll(double newRoll);
        Camera& setPos(Presto::vec3 newPos);
        Camera& setFocus(Presto::vec3 newPos);

        [[nodiscard]] double getYaw() const;
        [[nodiscard]] double getPitch() const;
        [[nodiscard]] double getRoll() const;
        [[nodiscard]] Presto::vec3 getPos() const;
        [[nodiscard]] Presto::vec3 getFocus() const;

       private:
        CameraType type_;

        Presto::vec3 _cameraPos{0, 0, 0.75};

        Presto::vec3 _focusPoint{0, 0, 0};

        double _yaw{0.0};
        double _pitch{0.0};
        double _roll{0.0};

        void recalculate();

        glm::highp_mat4 _viewMatrix{1};
    };
}  // namespace Presto
