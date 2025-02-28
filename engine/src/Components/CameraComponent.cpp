#include "Presto/Objects/Components/CameraComponent.h"

#include "Presto/Core/Constants.h"

namespace Presto {
CameraComponent::CameraComponent() : LazyCalculator() { this->setDirty(); }

glm::mat4
CameraComponent::getViewMatrix() {  // Calculate offset pointing at origin
    if (this->isDirty()) {
        recalculate();
    }

    return viewMatrix_;
}

Presto::mat4 CameraComponent::getProjectionMatrix() {
    if (this->isDirty()) {
        recalculate();
    }

    return projectionMatrix_;
};

CameraComponent& CameraComponent::setYaw(double newYaw) {
    _yaw = newYaw;
    setDirty();

    return *this;
}
CameraComponent& CameraComponent::setPitch(double newPitch) {
    _pitch = newPitch;
    setDirty();

    return *this;
}
CameraComponent& CameraComponent::setRoll(double newRoll) {
    _roll = newRoll;
    setDirty();

    return *this;
}
CameraComponent& CameraComponent::setPos(glm::vec3 newPos) {
    _cameraPos = newPos;
    setDirty();

    return *this;
}

CameraComponent& CameraComponent::setFocus(glm::vec3 newPos) {
    _focusPoint = newPos;
    setDirty();

    return *this;
}

double CameraComponent::getYaw() const { return _yaw; }
double CameraComponent::getPitch() const { return _pitch; }
double CameraComponent::getRoll() const { return _roll; }
glm::vec3 CameraComponent::getPos() const { return _cameraPos; }
glm::vec3 CameraComponent::getFocus() const { return _focusPoint; }

void CameraComponent::recalculate() {
    // Calculate offset pointing at origin with the y axis up
    viewMatrix_ = glm::lookAt(_cameraPos, _focusPoint, glm::vec3(0, 1, 0));

    projectionMatrix_ = glm::perspectiveFov<double>(
        fov_, extents_.width, extents_.height, near_, far_);
}

CameraComponent& CameraComponent::setFOV(camera_fov_t fovDegrees) {
    fov_ = glm::radians(fovDegrees);
    setDirty();

    return *this;
};

CameraComponent& CameraComponent::setDistances(camera_distance_t near,
                                               camera_distance_t far) {
    PR_ASSERT(near >= PR_MINIMUM_NEAR_DISTANCE,
              "The camera's near distance must be at least {}",
              PR_MINIMUM_NEAR_DISTANCE)
    PR_ASSERT(far > near,
              "The camera's far distance must be at least the near distance.")

    near_ = near;
    far_ = far;

    setDirty();

    return *this;
};
CameraComponent& CameraComponent::setExtents(VisualExtents newExtents) {
    extents_ = newExtents;
    setDirty();

    return *this;
}
}  // namespace Presto
