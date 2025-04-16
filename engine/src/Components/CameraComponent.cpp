#include "Presto/Objects/Components/CameraComponent.h"

namespace Presto {
CameraComponent::CameraComponent() : LazyCalculator() { this->setDirty(); }

mat4 CameraComponent::getViewMatrix() {  // Calculate offset pointing at origin
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
    transform_.rotation.y = newYaw;

    setDirty();
    return *this;
}
CameraComponent& CameraComponent::setPitch(double newPitch) {
    transform_.rotation.x = newPitch;

    setDirty();
    return *this;
}
CameraComponent& CameraComponent::setRoll(double newRoll) {
    transform_.rotation.z = newRoll;

    setDirty();
    return *this;
}
CameraComponent& CameraComponent::setPosition(vec3 newPos) {
    transform_.position = newPos;

    setDirty();
    return *this;
}

CameraComponent& CameraComponent::setFocus() {
    useFocus_ = false;

    setDirty();
    return *this;
};

CameraComponent& CameraComponent::setFocus(vec3 newPos) {
    useFocus_ = true;
    focusPoint_ = newPos;

    setDirty();
    return *this;
}

double CameraComponent::yaw() const { return transform_.rotation.y; }
double CameraComponent::pitch() const { return transform_.rotation.x; }
double CameraComponent::roll() const { return transform_.rotation.z; }
vec3 CameraComponent::position() const { return transform_.position; }
vec3 CameraComponent::rotation() const { return transform_.rotation; }

void CameraComponent::recalculate() {
    // Calculate offset pointing at origin with the y axis up
    if (type_ == CameraType::PERSPECTIVE) {
        if (useFocus_) {
            viewMatrix_ =
                glm::lookAt(transform_.position, focusPoint_, vec3(0, 1, 0));
        } else {
            viewMatrix_ = glm::inverse(transform_.asModelMat());
        }

        projectionMatrix_ =
            glm::perspectiveFov<double>(fov_, extents_.width, extents_.height,
                                        distances_.near, distances_.far);
    } else if (type_ == CameraType::ORTHOGRAPHIC) {
        viewMatrix_ = mat4{1};
        projectionMatrix_ =
            glm::ortho(-(extents_.width / 2.0F), (extents_.width / 2.0F),
                       -(extents_.height / 2.0F), (extents_.height / 2.0F),
                       distances_.near, distances_.far);
    }
}

CameraComponent& CameraComponent::setFOV(camera_fov_t fovDegrees) {
    fov_ = glm::radians(fovDegrees);

    setDirty();
    return *this;
};

CameraComponent& CameraComponent::setDistances(CameraDistances distances) {
    if (type_ == CameraType::PERSPECTIVE) {
        // PR_ASSERT(distances.near >= PR_MIN_NEAR_DISTANCE,
        // "The camera's near distance must be at least {}",
        // PR_MIN_NEAR_DISTANCE)
    }

    PR_ASSERT(distances.far > distances.near,
              "The camera's far distance must be at least the near distance.")

    distances_ = distances;

    setDirty();
    return *this;
};

CameraComponent& CameraComponent::setExtents(VisualExtents newExtents) {
    extents_ = newExtents;

    setDirty();
    return *this;
}

CameraComponent& CameraComponent::setType(CameraType newType) {
    this->type_ = newType;

    this->setDirty();
    return *this;
};

CameraType& CameraComponent::type() { return type_; };
VisualExtents& CameraComponent::extents() { return extents_; };

CameraComponent& CameraComponent::setRotation(Presto::vec3 rot) {
    transform_.rotation = rot;

    this->setDirty();
    return *this;
};

CameraComponent::camera_fov_t CameraComponent::FOV() const { return fov_; };

Presto::vec3 CameraComponent::focus() const { return focusPoint_; };

}  // namespace Presto
