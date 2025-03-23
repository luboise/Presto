#pragma once

namespace Presto {

struct TransformData {
    Presto::vec3 position;

    /* x: Yaw, y: Pitch, z: Roll */
    Presto::vec3 rotation;
    Presto::vec3 scale;

    [[nodiscard]] Presto::mat4 asMat4() const;

    TransformData& addRotation(Presto::vec3);
    TransformData& addTranslation(Presto::vec3);
    TransformData& scaleBy(Presto::vec3);
};

}  // namespace Presto
