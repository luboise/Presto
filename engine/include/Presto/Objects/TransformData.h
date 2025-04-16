#pragma once

namespace Presto {

struct TransformData {
    Presto::vec3 position;

    /* x: Yaw, y: Pitch, z: Roll */
    Presto::vec3 rotation;
    Presto::vec3 scale{1};

    [[nodiscard]] Presto::mat4 asModelMat() const;

    TransformData& addRotation(Presto::vec3);
    TransformData& addTranslation(Presto::vec3);
    TransformData& scaleBy(Presto::vec3);
};

}  // namespace Presto
