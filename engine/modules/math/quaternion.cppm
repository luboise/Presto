export module presto.math:quaternion;

import presto.types.core;

export namespace Pr {

struct Quaternion {
    float w{};
    Pr::vec3 xyz{};

    Quaternion();
    Quaternion(float angleDeg, Pr::vec3 axis);

    Quaternion operator*(const Quaternion& q) const;

    [[nodiscard]] Quaternion conjugate() const;

    static Quaternion newNonUnit(Pr::vec3 v);

    static Quaternion fromEuler(Pr::vec3 v);
    static Quaternion fromEuler(Pr::float32_t xPitch, Pr::float32_t yYaw,
                                Pr::float32_t zRoll);

    [[nodiscard]] Pr::vec3 toEuler() const;
    [[nodiscard]] Pr::mat3 toMat3() const;
    [[nodiscard]] Pr::mat4 toMat4() const;
};

Pr::vec3 operator*(const Quaternion&, const Pr::vec3&);
Pr::vec3 operator*(const Pr::vec3&, const Quaternion&);

}  // namespace Pr
