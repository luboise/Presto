#pragma once

#include "Presto/Objects/Component.h"

#include "Presto/Utils/LazyCalculator.h"

namespace Presto {
enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

class CameraComponent : public Component, LazyCalculator {
    using camera_fov_t = double;
    using camera_distance_t = float;

    friend class EntityManager;
    friend class EntityManagerImpl;

   public:
    struct CameraDistances {
        camera_distance_t near;
        camera_distance_t far;
    };

    [[nodiscard]] Presto::mat4 getViewMatrix();
    [[nodiscard]] Presto::mat4 getProjectionMatrix();

    CameraComponent& setType(CameraType newType);

    CameraComponent& setYaw(double newYaw);
    CameraComponent& setPitch(double newPitch);
    CameraComponent& setRoll(double newRoll);
    CameraComponent& setPos(Presto::vec3 newPos);
    CameraComponent& setFocus(Presto::vec3 newPos);

    CameraComponent& setExtents(VisualExtents newExtents);

    // Sets the camera's FOV to a new value in degrees
    CameraComponent& setFOV(camera_fov_t fovDegrees);

    // Sets the near and far planes of a perspective projection.
    // Requires near >= PR_MIN_NEAR_DISTANCE, and far >= near
    CameraComponent& setDistances(CameraDistances distances);

    [[nodiscard]] CameraDistances getDistances() const { return distances_; }

    [[nodiscard]] VisualExtents& getExtents();

    [[nodiscard]] CameraType& getType();

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
    CameraDistances distances_{.near = 0.1, .far = 1000};

    // TODO: Move the default extents somewhere else
    // Default extents of 1080p
    VisualExtents extents_{.width = 1920, .height = 1080};

    double _yaw{0.0};
    double _pitch{0.0};
    double _roll{0.0};

    void recalculate();

    glm::mat4 viewMatrix_{1};
    glm::mat4 projectionMatrix_{1};
};
}  // namespace Presto
