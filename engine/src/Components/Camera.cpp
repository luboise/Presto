#include "Presto/Components/Camera.h"

#include "Presto/Core/Constants.h"

namespace Presto {
    Camera::Camera() { this->setDirty(); }

    glm::mat4 Camera::getViewMatrix() {  // Calculate offset pointing at origin
        if (this->isDirty()) {
            recalculate();
        }

        return viewMatrix_;
    }

    Presto::mat4 Camera::getProjectionMatrix() {
        if (this->isDirty()) {
            recalculate();
        }

        return projectionMatrix_;
    };

    Camera& Camera::setYaw(double newYaw) {
        _yaw = newYaw;
        setDirty();

        return *this;
    }
    Camera& Camera::setPitch(double newPitch) {
        _pitch = newPitch;
        setDirty();

        return *this;
    }
    Camera& Camera::setRoll(double newRoll) {
        _roll = newRoll;
        setDirty();

        return *this;
    }
    Camera& Camera::setPos(glm::vec3 newPos) {
        _cameraPos = newPos;
        setDirty();

        return *this;
    }

    Camera& Camera::setFocus(glm::vec3 newPos) {
        _focusPoint = newPos;
        setDirty();

        return *this;
    }

    double Camera::getYaw() const { return _yaw; }
    double Camera::getPitch() const { return _pitch; }
    double Camera::getRoll() const { return _roll; }
    glm::vec3 Camera::getPos() const { return _cameraPos; }
    glm::vec3 Camera::getFocus() const { return _focusPoint; }

    void Camera::recalculate() {
        // Calculate offset pointing at origin with the y axis up
        viewMatrix_ = glm::lookAt(_cameraPos, _focusPoint, glm::vec3(0, 1, 0));

        projectionMatrix_ = glm::perspectiveFov<double>(
            fov_, extents_.width, extents_.height, near_, far_);
    }

    Camera& Camera::setFOV(camera_fov_t fovDegrees) {
        fov_ = glm::radians(fovDegrees);
        setDirty();

        return *this;
    };

    Camera& Camera::setDistances(camera_distance_t near,
                                 camera_distance_t far) {
        PR_ASSERT(near >= 0, "The camera's near distance must be at least {}",
                  PR_MINIMUM_NEAR_DISTANCE)
        near_ = near;
        far_ = far;

        setDirty();

        return *this;
    };
    Camera& Camera::setExtents(VisualExtents newExtents) {
        extents_ = newExtents;
        setDirty();

        return *this;
    }
}  // namespace Presto
