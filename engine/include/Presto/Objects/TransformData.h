#pragma once

#include "Presto/Math.h"

namespace Presto {

vec3 applyRotations(const vec3&, const vec3&);
vec3 applyTransformation(const vec3&, const mat4&);

struct TransformData {
    Presto::vec3 position;
    Quaternion rotation;

    TransformData();
    explicit TransformData(Presto::vec3 position, Quaternion rotation = {});
    TransformData(Presto::vec3 position, Presto::vec3 rotation);

    Presto::vec3 scale{1, 1, 1};

    [[nodiscard]] Presto::mat4 asModelMat() const;
    [[nodiscard]] Presto::mat4 asViewMat() const;

    [[nodiscard]] Presto::vec3 forwards() const;
    [[nodiscard]] Presto::vec3 backwards() const;

    [[nodiscard]] Presto::vec3 leftwards() const;
    [[nodiscard]] Presto::vec3 rightwards() const;

    [[nodiscard]] Presto::vec3 upwards() const;
    [[nodiscard]] Presto::vec3 downwards() const;

    TransformData& addRotation(Presto::vec3);
    TransformData& addRotation(Presto::Quaternion);

    TransformData& addTranslation(Presto::vec3);
    TransformData& scaleBy(Presto::vec3);
};

}  // namespace Presto
