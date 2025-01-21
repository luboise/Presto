#include "Presto/Components/Transform.h"
#include <algorithm>
#include "Presto/Modules/EntityManager.h"

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

    void Transform::translate(vec3 translation) {
        this->translation_ += translation;
        if (std::ranges::any_of(useRounding_, [](bool val) { return val; })) {
            this->round();
        }
    };

    void Transform::rotate(double x, double y, double z) {
        yawPitchRoll_ += vec3(x, y, z);
    }

    Transform* Transform::New() {
        auto& em{EntityManager::Get()};
        return em.newComponent<Transform>();
    }

    void Transform::round() {
        this->translation_ = {useRounding_[0] ? std::round(this->translation_.x)
                                              : this->translation_.x,
                              useRounding_[1] ? std::round(this->translation_.y)
                                              : this->translation_.y,
                              useRounding_[2] ? std::round(this->translation_.z)
                                              : this->translation_.z};
    };
    void Transform::setScale(vec3 scale) { this->scale_ = scale; };
}  // namespace Presto
