#include "Presto/Objects/Components/TransformComponent.h"
#include "Presto/Objects/TransformData.h"

namespace Presto {

mat4 TransformData::asModelMat() const {
    mat4 model{1.0F};

    model = glm::translate(model, this->position);

    model = model * rotation.toMat4();

    /*
// Apply in reverse order to avoid gimbal lock
model = glm::rotate(model, glm::radians(this->rotation.z), vec3(0, 0, 1));
model = glm::rotate(model, glm::radians(this->rotation.y), vec3(0, 1, 0));
model = glm::rotate(model, glm::radians(this->rotation.x), vec3(1, 0, 0));
    */

    model = glm::scale(model, this->scale);

    return model;
}

mat4 TransformData::asViewMat() const { return glm::inverse(asModelMat()); }

mat4 TransformComponent::getModelMatrix(vec3 offset, vec3 yawPitchRoll,
                                        vec3 scale) {
    mat4 model{1.0F};

    model = glm::translate(model, offset);

    model = glm::rotate(model, glm::radians(yawPitchRoll.x), vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(yawPitchRoll.y), vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(yawPitchRoll.z), vec3(0, 0, 1));

    model = glm::scale(model, scale);

    return model;
}

TransformComponent& TransformComponent::translate(vec3 translation) {
    this->transformData_.addTranslation(translation);
    /*
if (std::ranges::any_of(useRounding_, [](bool val) { return val; })) {
this->round();
}
    */
    return *this;
};

TransformComponent& TransformComponent::rotate(vec3 rotation) {
    this->transformData_.addRotation(rotation);
    return *this;
}

TransformComponent& TransformComponent::rotate(double x, double y, double z) {
    this->transformData_.addRotation({x, y, z});
    return *this;
}

/*
Transform& Transform::round() {
this->translation_ = {useRounding_[0] ? std::round(this->translation_.x)
                              : this->translation_.x,
              useRounding_[1] ? std::round(this->translation_.y)
                              : this->translation_.y,
              useRounding_[2] ? std::round(this->translation_.z)
                              : this->translation_.z};
};
                                                                          */

TransformComponent& TransformComponent::setScale(float scale) {
    transformData_.scale = vec3{scale};
    return *this;
};

TransformComponent& TransformComponent::setScale(vec3 scale) {
    transformData_.scale = scale;
    return *this;
};
vec3 TransformComponent::getPosition() const { return transformData_.position; }

TransformComponent& TransformComponent::setTranslation(vec3 translation) {
    transformData_.position = translation;
    return *this;
};

TransformComponent& TransformComponent::setRotation(vec3 yawPitchRoll) {
    transformData_.rotation = Quaternion::fromEuler(yawPitchRoll);
    return *this;
};

Quaternion TransformComponent::getRotation() const {
    return transformData_.rotation;
};
vec3 applyTransformation(const vec3& v, const mat4& transformations) {
    vec4 transformed = transformations * vec4{v, 1};
    transformed /= transformed.w;

    return vec3{transformed};
};

const TransformData& TransformComponent::data() const { return transformData_; }

TransformData::TransformData() : TransformData({0, 0, 0}, Quaternion{}) {};

TransformData::TransformData(Presto::vec3 position, Quaternion rotation)
    : position{position}, rotation{rotation} {};

TransformData::TransformData(Presto::vec3 position, Presto::vec3 rotation)
    : position{position}, rotation{Quaternion::fromEuler(rotation)} {};

}  // namespace Presto
