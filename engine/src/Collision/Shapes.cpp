#include "Presto/Collision/Shapes.h"

#include <glm/gtx/closest_point.hpp>

bool Presto::Intersects(const Presto::Cylinder& cylinder,
                        Presto::Triangle tri) {
    using namespace Presto;

    // View the triangle from the cylinder's perspective (from above)
    tri *= glm::inverse(cylinder.data.asMat4());

    // Remove y components (2d triangle)
    tri.p1.y = 0;
    tri.p2.y = 0;
    tri.p3.y = 0;

    // Now we want to find the shortest distance to the origin, because the
    // cylinder originates at (0,0,0)

    const Point2D origin{0, 0};

    bool top_check{false};

    if (glm::length(glm::closestPointOnLine(vec3{0}, tri.p2, tri.p1)) <=
            cylinder.radius ||
        glm::length(glm::closestPointOnLine(vec3{0}, tri.p3, tri.p2)) <=
            cylinder.radius ||
        glm::length(glm::closestPointOnLine(vec3{0}, tri.p3, tri.p1)) <=
            cylinder.radius) {
        top_check = true;
    }

    if (!top_check) {
        return false;
    }

    // Can treat cylinder as a circle
    // Can treat tri as a 2D triangle (ignore y)

    bool check1 = false;
    ClosestPointTo({tri.p1, tri.p2}, const Point2D&)
};

Presto::Triangle& Presto::Triangle::operator*=(const Presto::mat4& other) {
    *this = *this * other;
    return *this;
}

Presto::Triangle Presto::Triangle::operator*(const Presto::mat4& other) const {
    // TODO: Test the performance of this
    return {
        .p1 = vec3{vec4{this->p1, 1} * other},
        .p2 = vec3{vec4{this->p2, 2} * other},
        .p3 = vec3{vec4{this->p3, 3} * other},
    };
}
Presto::float32_t Presto::LineSegment3D::length() const {
    return ShortestDistance(p1, p2);
}
Presto::float32_t Presto::ShortestDistance(const Point3D& p1,
                                           const Point3D& p2) {
    return glm::length(p2 - p1);
}

Presto::vec2 Presto::ClosestPointTo(const LineSegment2D& segment,
                                    const Point2D& point) {
    auto norm{NormalOf(segment, point)};
};
