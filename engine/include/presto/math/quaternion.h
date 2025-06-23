#include "presto/core/types.h"

namespace Presto {

struct Quaternion {
    float w{};
    Presto::vec3 xyz{};

    Quaternion();
    Quaternion(float angleDeg, Presto::vec3 axis);

    Quaternion operator*(const Quaternion& q) const;

    [[nodiscard]] Quaternion conjugate() const;

    static Quaternion newNonUnit(Presto::vec3 v);

    static Quaternion fromEuler(Presto::vec3 v);
    static Quaternion fromEuler(Presto::float32_t xPitch,
                                Presto::float32_t yYaw,
                                Presto::float32_t zRoll);

    [[nodiscard]] Presto::vec3 toEuler() const;
    [[nodiscard]] Presto::mat3 toMat3() const;
    [[nodiscard]] Presto::mat4 toMat4() const;
};

Presto::vec3 operator*(const Quaternion&, const Presto::vec3&);
Presto::vec3 operator*(const Presto::vec3&, const Quaternion&);

}  // namespace Presto
