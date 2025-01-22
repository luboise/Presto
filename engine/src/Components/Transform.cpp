#include "Presto/Components/Transform.h"

namespace Presto {
    mat4 Transform::getModelMatrix(vec3 offset, vec3 yawPitchRoll, vec3 scale) {
        mat4 model{1.0F};

        model = glm::rotate(model, glm::radians(yawPitchRoll.x), vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(yawPitchRoll.y), vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(yawPitchRoll.z), vec3(0, 0, 1));

        model = glm::translate(model, offset);
        model = glm::scale(model, scale);

        return model;
    }

    Transform& Transform::translate(vec3 translation) {
        this->translation_ += translation;
        /*
if (std::ranges::any_of(useRounding_, [](bool val) { return val; })) {
    this->round();
}
        */
        return *this;
    };

    Transform& Transform::rotate(double x, double y, double z) {
        yawPitchRoll_ += vec3(x, y, z);
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

    Transform& Transform::setScale(vec3 scale) {
        this->scale_ = scale;
        return *this;
    };
}  // namespace Presto
