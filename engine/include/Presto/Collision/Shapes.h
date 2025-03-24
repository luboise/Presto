#pragma once

#include "Presto/Core.h"
#include "Presto/Objects/TransformData.h"

namespace Presto {

using Point2D = vec2;
using Point = vec3;

Presto::float32_t ShortestDistance(const Point& p1, const Point& p2);

struct Shape {
    TransformData data{};

    bool contains(Point p);
};

template <typename T>
struct LineSegmentBase {
    T p1;
    T p2;

    [[nodiscard]] Presto::float32_t length() const {
        return glm::length(p2 - p1);
    }
};

using LineSegment = LineSegmentBase<vec3>;
using LineSegment2D = LineSegmentBase<vec2>;

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

struct Ray {
    vec3 position;
    Presto::float32_t magnitude;
    vec3 direction;
};

struct Rectangle : Shape {
    // Give any 2 points
    Rectangle(vec2 p1, vec2 p2);

    vec2 bottom_left{};
    vec2 top_right{};
};

struct Triangle : Shape {
    Point p1;
    Point p2;
    Point p3;

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

bool Intersects2D(const Rectangle& rect, Point2D point);
bool Intersects2D(const Rectangle& rect, LineSegment2D segment);

vec3 ClosestPointTo(const LineSegment&, const Point&);
// vec2 ClosestPointTo(const LineSegment2D&, const Point2D&);

}  // namespace Presto
