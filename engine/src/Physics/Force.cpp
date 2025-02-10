#include "Presto/Physics/Force.h"

namespace Presto {
Force operator+(Force& left, Force& right) {
    const Force new_force{
        .velocity = left.velocity + right.velocity,
        .angular_velocity = left.angular_velocity + right.angular_velocity};

    return new_force;
}

Force& operator+=(Force& left, Force& right) {
    left.velocity += right.velocity;
    left.angular_velocity += right.angular_velocity;

    return left;
}

Force operator*(Force force, float scalar) {
    return Force{.velocity = force.velocity * scalar,
                 .angular_velocity = force.angular_velocity * scalar};
};
Force operator/(Force force, float scalar) {
    return {.velocity = force.velocity / scalar,
            .angular_velocity = force.angular_velocity / scalar};
};
Force& operator*=(Force& force, float scalar) {
    force.velocity *= scalar;
    force.angular_velocity *= scalar;

    return force;
};
Force& operator/=(Force& force, double scalar) {
    force.velocity /= scalar;
    force.angular_velocity /= scalar;

    return force;
};
}  // namespace Presto
