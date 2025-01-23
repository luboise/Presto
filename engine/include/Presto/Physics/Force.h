#pragma once

namespace Presto {
    struct Force {
        vec3 velocity{0};
        vec3 angular_velocity{0};
    };

    // TODO: Work out operator semantics

    Force operator+(Force& left, Force& right);
    Force& operator+=(Force& left, Force& right);

    Force operator/(Force force, float scalar);
    Force& operator/=(Force& force, double scalar);

    Force operator*(Force force, float scalar);
    Force& operator*=(Force& force, float scalar);

}  // namespace Presto
