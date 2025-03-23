#pragma once

#include "Presto/Objects/TransformData.h"

namespace Presto {

using Point2D = vec2;
using Point3D = vec3;

Presto::float32_t ShortestDistance(const Point3D& p1, const Point3D& p2);

struct Shape {
    TransformData data{};

    bool contains(Point3D p);
};

template <typename T>
struct LineSegmentBase {
    T p1;
    T p2;

    [[nodiscard]] Presto::float32_t length() const {
        return glm::length(p2 - p1);
    }
};

using LineSegment2D = LineSegmentBase<Point2D>;
using LineSegment3D = LineSegmentBase<Point3D>;

using LineSegment = LineSegment3D;

/*
struct LineSegment3D {
    Point3D p1;
    Point3D p2;

    [[nodiscard]] Presto::float32_t length() const;
};
*/

Presto::vec2 NormalOf(LineSegment2D segment) {
    vec2 subtracted{segment.p2 - segment.p1};

    // Get the normal of the segment
    return {-subtracted.y, subtracted.x};
}

struct Circle : Shape {
    Presto::float32_t radius{1};
};

struct Triangle : Shape {
    Point3D p1;
    Point3D p2;
    Point3D p3;

    [[nodiscard]] Triangle operator*(const Presto::mat4& other) const;
    Triangle& operator*=(const Presto::mat4& other);
};

// A cylinder shape, assumed to be pointing straight upwards (towards y)
struct Cylinder : Shape {
    Presto::float32_t radius{0.5};
    Presto::float32_t height{2};
};

// Checks if a cylinder intersects a triangle
bool Intersects(const Cylinder&, Triangle);
bool Intersects(const Circle&, const LineSegment&);

// vec3 ClosestPointTo(const LineSegment3D&, const Point3D&);
vec2 ClosestPointTo(const LineSegment2D&, const Point2D&);

}  // namespace Presto
