#include "Camera.h"

namespace Presto {
    Camera::Camera() { this->recalculate(); }

    glm::mat4 Camera::getViewMatrix()
        const {  // Calculate offset pointing at origin
        return _viewMatrix;
    }

    void Camera::setYaw(double newYaw) {
        _yaw = newYaw;
        recalculate();
    }
    void Camera::setPitch(double newPitch) {
        _pitch = newPitch;
        recalculate();
    }
    void Camera::setRoll(double newRoll) {
        _roll = newRoll;
        recalculate();
    }
    void Camera::setPos(glm::vec3 newPos) {
        _cameraPos = newPos;
        recalculate();
    }

    void Camera::setFocus(glm::vec3 newPos) {
        _focusPoint = newPos;
        recalculate();
    }

    double Camera::getYaw() const { return _yaw; }
    double Camera::getPitch() const { return _pitch; }
    double Camera::getRoll() const { return _roll; }
    glm::vec3 Camera::getPos() const { return _cameraPos; }
    glm::vec3 Camera::getFocus() const { return _focusPoint; }

    void Camera::recalculate() {
        // Calculate offset pointing at origin with the y axis up
        _viewMatrix = glm::lookAt(_cameraPos, _focusPoint, glm::vec3(0, 1, 0));
    }
}  // namespace Presto