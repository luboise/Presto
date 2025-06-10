#include <cmath>

#include "Presto/Math.h"

namespace Presto {

Quaternion::Quaternion() : Quaternion(0, {1, 0, 0}) {};

Quaternion::Quaternion(float angleDeg, Presto::vec3 axis) {
    float half_angle{glm::radians(angleDeg / 2)};

    this->w = std::cos(half_angle);
    this->xyz = std::sin(half_angle) * glm::normalize(axis);
};

Quaternion Quaternion::operator*(const Quaternion& q) const {
    const Quaternion& p = *this;

    // From expansion of (w + xi + yj + kz) * (w + xi + yj + zk)
    float new_w{(p.w * q.w) - glm::dot(p.xyz, q.xyz)};
    vec3 new_xyz{p.w * q.xyz + q.w * p.xyz};

    new_xyz += glm::cross(p.xyz, q.xyz);

    Quaternion ret{};
    ret.w = new_w;
    ret.xyz = new_xyz;

    return ret;
}

Quaternion Quaternion::fromEuler(Presto::float32_t xPitch,
                                 Presto::float32_t yYaw,
                                 Presto::float32_t zRoll) {
    glm::vec3 e = glm::radians(glm::vec3(xPitch, yYaw, zRoll));

    glm::vec3 c = glm::cos(e * 0.5F);
    glm::vec3 s = glm::sin(e * 0.5F);

    Quaternion q;
    q.w = c.x * c.y * c.z + s.x * s.y * s.z;
    q.xyz.x = s.x * c.y * c.z - c.x * s.y * s.z;
    q.xyz.y = c.x * s.y * c.z + s.x * c.y * s.z;
    q.xyz.z = c.x * c.y * s.z - s.x * s.y * c.z;

    return q;
}

Quaternion Quaternion::fromEuler(Presto::vec3 v) {
    return fromEuler(v.x, v.y, v.z);
};

Presto::mat3 Quaternion::toMat3() const {  // Precompute products
    float x2 = xyz.x + xyz.x;
    float y2 = xyz.y + xyz.y;
    float z2 = xyz.z + xyz.z;

    float xx = xyz.x * x2;
    float yy = xyz.y * y2;
    float zz = xyz.z * z2;
    float xy = xyz.x * y2;
    float xz = xyz.x * z2;
    float yz = xyz.y * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

    // Transpose because glm is column order
    return glm::transpose(mat3{
        {1.0F - (yy + zz), xy - wz, xz + wy},
        {xy + wz, 1.0F - (xx + zz), yz - wx},
        {xz - wy, yz + wx, 1.0F - (xx + yy)},
    });
};

Presto::mat4 Quaternion::toMat4() const {  // Precompute products
    return {this->toMat3()};
}

Presto::vec3 Quaternion::toEuler() const {
    float x = this->xyz.x;
    float y = this->xyz.y;
    float z = this->xyz.z;

    float sinr_cosp = 2.0F * (w * x + y * z);
    float cosr_cosp = 1.0F - (2.0F * (x * x + y * y));
    float roll = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0F * (w * y - z * x);
    float pitch = NAN;
    if (std::abs(sinp) >= 1.0F) {
        pitch = std::copysign<float>(M_PI / 2.0F,
                                     sinp);  // use 90 degrees if out of range
    } else {
        pitch = std::asin(sinp);
    }

    float siny_cosp = 2.0F * (w * z + x * y);
    float cosy_cosp = 1.0F - (2.0F * (y * y + z * z));
    float yaw = std::atan2(siny_cosp, cosy_cosp);

    return {glm::degrees(yaw), glm::degrees(pitch), glm::degrees(roll)};
};

Presto::vec3 operator*(const Quaternion& q, const Presto::vec3& v) {
    return (q * Quaternion::newNonUnit(v) * q.conjugate()).xyz;
};

Presto::vec3 operator*(const Presto::vec3& v, const Quaternion& q) {
    return (q.conjugate() * Quaternion::newNonUnit(v) * q).xyz;
};

Quaternion Quaternion::conjugate() const {
    // TODO: Implement this for non-unit quaternions

    Quaternion new_quat{*this};
    new_quat.xyz *= -1;

    return new_quat;
};

Quaternion Quaternion::newNonUnit(Presto::vec3 v) {
    Quaternion q{};
    q.w = 0;
    q.xyz = v;
    return q;
}
}  // namespace Presto
