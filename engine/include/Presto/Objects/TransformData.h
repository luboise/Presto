#pragma once

namespace Presto {

vec3 applyRotations(const vec3&, const vec3&);
vec3 applyTransformation(const vec3&, const mat4&);

struct TransformData {
    Presto::vec3 position;

    /* x: Pitch, y: Yaw, z: Roll */
    Presto::vec3 rotation;
    Presto::vec3 scale{1, 1, 1};

    [[nodiscard]] Presto::mat4 asModelMat() const;
    [[nodiscard]] Presto::mat4 asWorldMat() const;
    [[nodiscard]] Presto::mat4 asViewMat() const;

    [[nodiscard]] Presto::vec3 forwards() const;

    TransformData& addRotation(Presto::vec3);
    TransformData& addTranslation(Presto::vec3);
    TransformData& scaleBy(Presto::vec3);
};

}  // namespace Presto
