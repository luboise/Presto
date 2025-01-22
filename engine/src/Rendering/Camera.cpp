#include "Presto/Rendering/Camera.h"

namespace Presto {
    Camera::Camera() { this->recalculate(); }

    glm::mat4 Camera::getViewMatrix()
        const {  // Calculate offset pointing at origin
        return _viewMatrix;
    }

    Camera& Camera::setYaw(double newYaw) {
        _yaw = newYaw;
        recalculate();

        return *this;
    }
    Camera& Camera::setPitch(double newPitch) {
        _pitch = newPitch;
        recalculate();

        return *this;
    }
    Camera& Camera::setRoll(double newRoll) {
        _roll = newRoll;
        recalculate();

        return *this;
    }
    Camera& Camera::setPos(glm::vec3 newPos) {
        _cameraPos = newPos;
        recalculate();

        return *this;
    }

    Camera& Camera::setFocus(glm::vec3 newPos) {
        _focusPoint = newPos;
        recalculate();

        return *this;
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
