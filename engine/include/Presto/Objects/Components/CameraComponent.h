#pragma once

#include "Presto/Objects/Component.h"

#include "Presto/Utils/LazyCalculator.h"

namespace Presto {
enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

class CameraComponent : public Component, LazyCalculator {
    using camera_fov_t = double;
    using camera_distance_t = double;

    friend class EntityManager;
    friend class EntityManagerImpl;

   public:
    [[nodiscard]] Presto::mat4 getViewMatrix();
    [[nodiscard]] Presto::mat4 getProjectionMatrix();

    CameraComponent& setYaw(double newYaw);
    CameraComponent& setPitch(double newPitch);
    CameraComponent& setRoll(double newRoll);
    CameraComponent& setPos(Presto::vec3 newPos);
    CameraComponent& setFocus(Presto::vec3 newPos);

    CameraComponent& setExtents(VisualExtents newExtents);

    // Sets the camera's FOV to a new value in degrees
    CameraComponent& setFOV(camera_fov_t fovDegrees);

    // Sets the near and far planes of a perspective projection.
    // Requires near >= PR_MINIMUM_NEAR_DISTANCE, and far >= near
    CameraComponent& setDistances(camera_distance_t newNear,
                                  camera_distance_t newFar);

    [[nodiscard]] double getYaw() const;
    [[nodiscard]] double getPitch() const;
    [[nodiscard]] double getRoll() const;
    [[nodiscard]] Presto::vec3 getPos() const;
    [[nodiscard]] Presto::vec3 getFocus() const;

   private:
    CameraComponent();

    CameraType type_{CameraType::PERSPECTIVE};

    Presto::vec3 _cameraPos{0, 0, 0.75};

    Presto::vec3 _focusPoint{0, 0, 0};

    // FOV of the camera (default of 90deg)
    camera_fov_t fov_{glm::radians(90.0F)};
    camera_distance_t near_{1};
    camera_distance_t far_{1000};

    // TODO: Move the default extents somewhere else
    // Default extents of 1080p
    VisualExtents extents_{.width = 1920, .height = 1080};

    double _yaw{0.0};
    double _pitch{0.0};
    double _roll{0.0};

    void recalculate();

    glm::highp_mat4 viewMatrix_{1};
    glm::highp_mat4 projectionMatrix_{1};
};
}  // namespace Presto
