module;
#include "presto/platform.h"

export module presto.objects.components.camera;

import presto.types;
import presto.core.constants;
import presto.math;
import presto.utils;
import presto.objects.base;

import std;

export namespace Pr {

enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

class PRESTO_API CameraComponent : public Component, LazyCalculator {
    using camera_fov_t = double;
    using camera_distance_t = float;

   public:
    CameraComponent();

    struct CameraDistances {
        camera_distance_t near;
        camera_distance_t far;
    };

    [[nodiscard]] Pr::mat4 getViewMatrix();
    [[nodiscard]] Pr::mat4 getProjectionMatrix();

    CameraComponent& setType(CameraType newType);

    CameraComponent& setFocus(Pr::vec3 newPos);
    CameraComponent& setFocus();

    [[nodiscard]] Pr::vec3 focus() const;

    CameraComponent& setExtents(VisualExtents newExtents);

    // Sets the camera's vertical FOV to a new value in degrees
    CameraComponent& setFOV(camera_fov_t fovDegrees);
    [[nodiscard]] camera_fov_t FOV() const;

    // Sets the near and far planes of a perspective projection.
    // Requires near >= PR_MIN_NEAR_DISTANCE, and far >= near
    CameraComponent& setDistances(CameraDistances distances);
    [[nodiscard]] CameraDistances distances() const { return distances_; }

    [[nodiscard]] VisualExtents& extents();
    [[nodiscard]] CameraType& type();

    [[nodiscard]] TransformData transformData() const;

    [[nodiscard]] Pr::vec3 position() const;
    CameraComponent& setPosition(vec3 newPos);

    template <typename... Args>
        requires std::constructible_from<Pr::vec3, Args...>
    CameraComponent& setPosition(Args&&... args) {
        return setPosition(Pr::vec3{std::forward<Args>(args)...});
    }

    CameraComponent& rotate(Pr::vec3 rot);

    [[nodiscard]] Quaternion rotation() const;
    CameraComponent& setRotation(Pr::vec3 rot);

    template <typename... Args>
        requires std::constructible_from<Pr::vec3, Args...>
    CameraComponent& setRotation(Args&&... args) {
        return setRotation(Pr::vec3{std::forward<Args>(args)...});
    }

    [[nodiscard]] Rectangle distanceRect(camera_distance_t distance) const;
    [[nodiscard]] Rectangle farRectangle() const;
    [[nodiscard]] Rectangle nearRectangle() const;

   private:
    CameraType type_{CameraType::PERSPECTIVE};

    bool useFocus_{false};
    Pr::vec3 focusPoint_{0, 0, 0};

    // FOV of the camera (default of 90deg)
    camera_fov_t verticalFov_{Pr::Math::Radians(90.0F)};
    CameraDistances distances_{.near = 0.1, .far = 1000};

    // TODO: Move the default extents somewhere else
    // Default extents of 1080p
    VisualExtents extents_{.width = 1920, .height = 1080};

    TransformData transform_{};

    void recalculate();

    Pr::mat4 viewMatrix_{1};
    Pr::mat4 projectionMatrix_{1};
};
}  // namespace Pr
