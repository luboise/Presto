#pragma once

#include "Presto/Core.h"
#include "Presto/Objects/TransformData.h"

namespace Presto {

struct Cylinder;
struct Triangle;
struct Circle;

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

using Point2D = vec2;
using Point = vec3;

Presto::float32_t ShortestDistance(const Point& p1, const Point& p2);

Presto::vec2 NormalOf(LineSegment2D segment);

template <typename A, typename B>
concept IntersectableWith = requires(const A& a, const B& b) {
    { Intersects(a, b) } -> std::same_as<bool>;
};

struct CollisionShape {
    TransformData data{};
    bool contains(Point p);
};

#define COLLISION_FUNCTIONS(T)
/*
#define COLLISION_FUNCTIONS(T)                                 \
    [[nodiscard]] bool intersects(const CollisionShape& other) \
        const override {                                       \
        return other.intersects(*this);                        \
    }                                                          \
    template <typename U>                                      \
        requires DerivedFrom<T, U> && IntersectableWith<T, U>  \
    [[nodiscard]] bool intersects(const U& other) {            \
        return Intersects(*this, other);                       \
    }                                                          \
                                                               \
    template <typename... Args>                                \
    explicit T(Args&&... args) : T(std::forward<Args>(args)...) {}
        */

struct Circle : CollisionShape {
    Presto::float32_t radius{1};
};

struct Ray {
    vec3 position;
    Presto::float32_t magnitude;
    vec3 direction;
};

struct Rectangle : CollisionShape {
    // Give any 2 points
    Rectangle(vec2 p1, vec2 p2);

    vec2 bottom_left{};
    vec2 top_right{};

    COLLISION_FUNCTIONS(Rectangle)
};

struct Triangle : CollisionShape {
    Point p1;
    Point p2;
    Point p3;

    [[nodiscard]] Triangle operator*(const Presto::mat4& other) const;
    Triangle& operator*=(const Presto::mat4& other);

    COLLISION_FUNCTIONS(Triangle)
};

// A cylinder shape, assumed to be pointing straight upwards (towards y)
struct Cylinder : CollisionShape {
    Presto::float32_t radius{0.5};
    Presto::float32_t height{2};

    COLLISION_FUNCTIONS(Cylinder)
};

// Checks if a cylinder intersects a triangle
bool Intersects(const Cylinder&, Triangle);
bool Intersects(const Circle&, const LineSegment&);

bool Intersects2D(const Rectangle& rect, Point2D point);
bool Intersects2D(const Rectangle& rect, LineSegment2D segment);

vec3 ClosestPointTo(const LineSegment&, const Point&);
// vec2 ClosestPointTo(const LineSegment2D&, const Point2D&);

}  // namespace Presto
