#pragma once

#include "Collision/Shapes.h"

namespace Pr {

struct CollisionResult {
    bool colliding{false};
    vec3 collision_point{};

    explicit operator bool() const { return colliding; }
};

CollisionResult Colliding(const Ray&, const Plane&);

}  // namespace Pr
