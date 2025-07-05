module presto.math;

import :transform_data;

import presto.core;

#include <glm/glm.hpp>

namespace Pr {

TransformData::TransformData() : TransformData({0, 0, 0}, Quaternion{}) {};

TransformData::TransformData(Pr::vec3 position, Quaternion rotation)
    : position{position}, rotation{rotation} {};

TransformData::TransformData(Pr::vec3 position, Pr::vec3 rotation)
    : position{position}, rotation{Quaternion::fromEuler(rotation)} {};

TransformData& TransformData::addRotation(Pr::Quaternion quat) {
    this->rotation = quat * this->rotation;
    return *this;
};

TransformData& TransformData::addTranslation(Pr::vec3 t) {
    this->position += t;
    return *this;
};
TransformData& TransformData::scaleBy(Pr::vec3 s) {
    this->scale *= s;
    return *this;
};

Pr::vec3 TransformData::forwards() const {
    return glm::normalize(rotation * vec3{0, 0, -1});
}

Pr::vec3 TransformData::backwards() const { return -forwards(); }

Pr::vec3 TransformData::leftwards() const {
    return glm::normalize(rotation * vec3{-1, 0, 0});
}

Pr::vec3 TransformData::rightwards() const { return -leftwards(); }

Pr::vec3 TransformData::upwards() const {
    return glm::normalize(rotation * vec3{0, 1, 0});
}

Pr::vec3 TransformData::downwards() const { return -upwards(); }

}  // namespace Pr
