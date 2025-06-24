#pragma once

#include <concepts>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

/*
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
*/

#include "presto/core/types.h"
#include "presto/objects.h"

#include "./transform_data.h"

namespace Pr {

struct Cylinder;
struct Triangle;
struct Circle;

template <typename T>
struct LineSegmentBase {
    T p1;
    T p2;

    [[nodiscard]] Pr::float32_t length() const { return glm::length(p2 - p1); }
};

using LineSegment = LineSegmentBase<vec3>;
using LineSegment2D = LineSegmentBase<vec2>;

using Point2D = vec2;
using Point = vec3;

Pr::float32_t ShortestDistance(const Point& p1, const Point& p2);

Pr::vec2 NormalOf(LineSegment2D segment);

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

struct Plane : CollisionShape {
    union {
        Pr::vec3 normal{0, 1, 0};
        Pr::vec3 N;
    };
    union {
        Pr::vec3 point{0, 0, 0};
        Pr::vec3 P;
    };
};

struct Circle : CollisionShape {
    Pr::float32_t radius{1};
};

struct Ray {
    union {
        Pr::vec3 origin;
        Pr::vec3 O;
    };

    union {
        Pr::float32_t magnitude;
        Pr::float32_t t;
    };

    union {
        Pr::vec3 direction;
        Pr::vec3 D;
    };

    [[nodiscard]] bool contains(Pr::float32_t tVal) const {
        return tVal >= 0 && tVal <= this->t;
    }

    [[nodiscard]] Pr::vec3 at(Pr::float32_t tVal) const { return O + tVal * D; }

    static Ray fromAB(Pr::vec3 a, Pr::vec3 b) {
        auto diff{b - a};

        return Ray{.origin = a,
                   .magnitude = glm::length(diff),
                   .direction = glm::normalize(diff)};
    };
};

struct Rectangle : CollisionShape {
    Rectangle() = default;
    Rectangle(Pr::vec3 topLeft, Pr::vec3 topRight, Pr::vec3 bottomLeft);

    Pr::vec3 top_left{0, 1, 0};
    Pr::vec3 top_right{1, 1, 0};
    Pr::vec3 bottom_left{0, 0, 0};

    [[nodiscard]] Pr::vec3 at(float x, float y) const;
    [[nodiscard]] Pr::vec3 at(vec2) const;

    [[nodiscard]] Pr::float32_t width() const;
    [[nodiscard]] Pr::float32_t height() const;

    COLLISION_FUNCTIONS(Rectangle)
};

struct Cube : CollisionShape {
    Cube() = default;
    explicit Cube(vec3 pos);
    [[nodiscard]] std::array<vec3, 8> vertices() const;

    COLLISION_FUNCTIONS(Cube)
};

struct Triangle : CollisionShape {
    Point p1;
    Point p2;
    Point p3;

    [[nodiscard]] Triangle operator*(const Pr::mat4& other) const;
    Triangle& operator*=(const Pr::mat4& other);

    COLLISION_FUNCTIONS(Triangle)
};

// A cylinder shape, assumed to be pointing straight upwards (towards y)
struct Cylinder : CollisionShape {
    Pr::float32_t radius{0.5};
    Pr::float32_t height{2};

    COLLISION_FUNCTIONS(Cylinder)
};

/*
// Checks if a cylinder intersects a triangle
bool Intersects(const Cylinder&, Triangle);
bool Intersects(const Circle&, const LineSegment&);

bool Intersects2D(const Rectangle& rect, Point2D point);
bool Intersects2D(const Rectangle& rect, LineSegment2D segment);

vec3 ClosestPointTo(const LineSegment&, const Point&);
// vec2 ClosestPointTo(const LineSegment2D&, const Point2D&);
*/

}  // namespace Pr
