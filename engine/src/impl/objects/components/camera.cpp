module presto.objects.components.camera;

#include "Presto/Collision/Shapes.h"
#include "Presto/Objects/Components/CameraComponent.h"
#include "Presto/Objects/TransformData.h"

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

vec3 CameraComponent::position() const { return transform_.position; }
Presto::Quaternion CameraComponent::rotation() const {
    return transform_.rotation;
}

void CameraComponent::recalculate() {
    // Calculate offset pointing at origin with the y axis up
    if (type_ == CameraType::PERSPECTIVE) {
        if (useFocus_) {
            viewMatrix_ =
                glm::lookAt(transform_.position, focusPoint_, vec3(0, 1, 0));
        } else {
            viewMatrix_ = transform_.asViewMat();
        }

        projectionMatrix_ = glm::perspectiveFov<double>(
            verticalFov_, extents_.width, extents_.height, distances_.near,
            distances_.far);
    } else if (type_ == CameraType::ORTHOGRAPHIC) {
        // viewMatrix_ = mat4{1};
        viewMatrix_ = transform_.asViewMat();

        projectionMatrix_ =
            glm::ortho(-(extents_.width / 2.0F), (extents_.width / 2.0F),
                       -(extents_.height / 2.0F), (extents_.height / 2.0F),
                       distances_.near, distances_.far);
    }
}

CameraComponent& CameraComponent::setFOV(camera_fov_t fovDegrees) {
    verticalFov_ = glm::radians(fovDegrees);

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
    transform_.rotation = Quaternion::fromEuler(rot);

    this->setDirty();
    return *this;
};

CameraComponent& CameraComponent::rotate(Presto::vec3 rot) {
    transform_.rotation = Quaternion::fromEuler(rot) * transform_.rotation;
    // transform_.rotation = transform_.rotation * Quaternion::fromEuler(rot);

    this->setDirty();
    return *this;
};

CameraComponent::camera_fov_t CameraComponent::FOV() const {
    return verticalFov_;
};

Presto::vec3 CameraComponent::focus() const { return focusPoint_; };

Rectangle CameraComponent::distanceRect(camera_distance_t distance) const {
    // Create a right angled triangle from the origin to where half of the width
    // of the rectangle would be in -z, ie,
    // tan(FOV/2) = w/2  /  FAR
    // Then solve that for w
    auto rec_height =
        static_cast<float>(std::tan(verticalFov_ / 2) * distance * 2);
    auto rec_width = static_cast<float>(rec_height * extents_.getAspectRatio());

    vec3 rec_center{transform_.position + transform_.forwards() * distance};

    vec3 top_left{rec_center + rec_width * 0.5F * transform_.leftwards() +
                  rec_height * transform_.upwards() * 0.5F};

    vec3 top_right{top_left + rec_width * transform_.rightwards()};
    vec3 bottom_left{top_left + rec_height * transform_.downwards()};

    Rectangle rec{top_left, top_right, bottom_left};

    return rec;
}

Rectangle CameraComponent::farRectangle() const {
    return this->distanceRect(distances_.far);
}

Rectangle CameraComponent::nearRectangle() const {
    return this->distanceRect(distances_.near);
}

TransformData CameraComponent::transformData() const { return transform_; }

}  // namespace Presto
