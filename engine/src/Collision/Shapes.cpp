#include "Presto/Collision/Shapes.h"

#include <glm/gtx/closest_point.hpp>

bool Presto::Intersects(const Presto::Cylinder& cylinder,
                        Presto::Triangle tri) {
    using namespace Presto;

    /*
// View the triangle from the cylinder's perspective (from above)

auto rotmat{cylinder.data.asMat4()};
rotmat = glm::inverse(rotmat);

tri *= rotmat;

Presto::Triangle copy{tri};

// Remove y components (2d triangle)
copy.p1.y = 0;
copy.p2.y = 0;
copy.p3.y = 0;

// Now we want to find the shortest distance to the origin, because the
// cylinder originates at (0,0,0)

const Point origin{0, 0, 0};

if (glm::length(ClosestPointTo(LineSegment(copy.p2, copy.p1), origin)) >
        cylinder.radius &&
    glm::length(ClosestPointTo(LineSegment(copy.p3, copy.p2), origin)) >
        cylinder.radius &&
    glm::length(ClosestPointTo(LineSegment(copy.p3, copy.p1), origin)) >
        cylinder.radius) {
    return false;
}

// Check the triangle from the front (no z value)
copy = tri;
copy.p1.z = 0;
copy.p2.z = 0;
copy.p3.z = 0;

Rectangle rec{{-cylinder.radius, -cylinder.height},
              {cylinder.radius, cylinder.height}};

if (Intersects2D(rec, LineSegment2D{copy.p1, copy.p2}) ||
    Intersects2D(rec, LineSegment2D{copy.p2, copy.p3}) ||
    Intersects2D(rec, LineSegment2D{copy.p1, copy.p3})) {
    return true;
}

copy = tri;

mat4 rotation_matrix{
    glm::rotate(mat4{1}, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f))};

copy.p1 = vec4{copy.p1, 1} * rotation_matrix;
copy.p1.z = 0;
copy.p2 = vec4{copy.p2, 1} * rotation_matrix;
copy.p2.z = 0;
copy.p3 = vec4{copy.p3, 1} * rotation_matrix;
copy.p3.z = 0;

return Intersects2D(rec, LineSegment2D{copy.p1, copy.p2}) ||
       Intersects2D(rec, LineSegment2D{copy.p2, copy.p3}) ||
       Intersects2D(rec, LineSegment2D{copy.p1, copy.p3});
               */
    return false;
};

Presto::Triangle& Presto::Triangle::operator*=(const Presto::mat4& other) {
    *this = *this * other;
    return *this;
}

Presto::Triangle Presto::Triangle::operator*(const Presto::mat4& other) const {
    // TODO: Test the performance of this
    return {
        .p1 = vec3{vec4{this->p1, 1} * other},
        .p2 = vec3{vec4{this->p2, 1} * other},
        .p3 = vec3{vec4{this->p3, 1} * other},
    };
}

template <>
Presto::float32_t Presto::LineSegment::length() const {
    return ShortestDistance(p1, p2);
}
Presto::float32_t Presto::ShortestDistance(const Point& p1, const Point& p2) {
    return glm::length(p2 - p1);
}

/*
Presto::vec2 Presto::ClosestPointTo(const LineSegment2D& segment,
                                    const Point2D& point) {
    auto norm{NormalOf(segment, point)};
};
*/

Presto::vec3 Presto::ClosestPointTo(const LineSegment& segment,
                                    const Point& point) {
    using namespace Presto;

    // Get P2 relative to the origin
    vec3 v{segment.p2 - segment.p1};

    // Get the point relative to the origin
    vec3 a{point - segment.p1};

    // Get the scalar projection of the point onto the line
    // 0 means its closest to the origin p1
    Presto::float32_t scalar_projection{glm::dot(a, v) / glm::dot(v, v)};

    if (scalar_projection <= 0) {
        return segment.p1;
    }
    if (scalar_projection >= 1) {
        return segment.p2;
    }

    return (scalar_projection * v) + segment.p1;
};

// Check if the point is inside of the rectangle
bool Presto::Intersects2D(const Rectangle& rect, Point2D point) {
    return rect.bottom_left.x <= point.x &&  //
           rect.bottom_left.y <= point.y &&  //
           point.x <= rect.top_right.x &&    //
           point.y <= rect.top_right.y;
};

bool Presto::Intersects2D(const Rectangle& rect, LineSegment2D segment) {
    if (Intersects2D(rect, segment.p1) || Intersects2D(rect, segment.p2)) {
        return true;
    }

    vec2 diff{segment.p2 - segment.p1};
    vec2 norm{glm::normalize(diff)};

    // Invert to avoid division in next step
    norm.x =
        norm.x == 0 ? std::numeric_limits<float>::infinity() : (1 / norm.x);
    norm.y =
        norm.y == 0 ? std::numeric_limits<float>::infinity() : (1 / norm.y);

    // Get the t values of all 4 sides of the rectangle
    vec2 left_bottom{(rect.bottom_left - segment.p1) * norm};
    vec2 right_top{(rect.top_right - segment.p1) * norm};

    float32_t entry_x{std::min(left_bottom.x, right_top.x)};
    float32_t entry_y{std::min(left_bottom.y, right_top.y)};
    float32_t exit_x{std::max(left_bottom.x, right_top.x)};
    float32_t exit_y{std::max(left_bottom.y, right_top.y)};

    float32_t t_entry{std::max(entry_x, entry_y)};
    float32_t t_exit{std::min(exit_x, exit_y)};

    if (t_entry < 0 || t_entry > t_exit) {
        return false;
    }

    float32_t t{t_entry < 0 ? t_exit : t_entry};
    float32_t sqr_length{(diff.x * diff.x) + (diff.y * diff.y)};

    return t > 0 && (t * t) <= sqr_length;

    // Cohen-Sutherland algorithm

    /*
    constexpr uint INSIDE = 0b0000;
    constexpr uint LEFT = 0b0001;
    constexpr uint RIGHT = 0b0010;
    constexpr uint BOTTOM = 0b0100;
    constexpr uint TOP = 0b1000;

auto get_outcode{[](float xmin, float xmax, float ymin, float ymax,
                    const vec2& point) -> uint {
    uint code{INSIDE};
    if (point.x < xmin) {
        code |= LEFT;
    } else if (point.x >= xmax) {
        code |= RIGHT;
    }
    if (point.y <= ymin) {
        code |= BOTTOM;
    } else if (point.y >= ymax) {
        code |= TOP;
    }
    return code;
}};

// Get outcode of each point
auto p1_outcode{get_outcode(rect.bottom_left.x, rect.top_right.x,
                            rect.bottom_left.y, rect.top_right.y,
                            segment.p1)};
auto p2_outcode{get_outcode(rect.bottom_left.x, rect.top_right.x,
                            rect.bottom_left.y, rect.top_right.y,
                            segment.p2)};

// If either point is inside the rectangle, trivial success
if ((p1_outcode | p2_outcode) == 0) {
    return true;
}
// If both points share a row/column, trivial failure
if ((p1_outcode & p2_outcode) != 0U) {
    return false;
}

// Figure out which one is higher
uint top_outcode{std::max(p1_outcode, p2_outcode)};
    */
};

Presto::Rectangle::Rectangle(vec2 p1, vec2 p2) {
    auto min_x{p1.x};
    auto max_x{p2.x};
    if (max_x < min_x) {
        auto temp{min_x};
        min_x = max_x;
        min_x = temp;
    }

    auto min_y{p1.y};
    auto max_y{p2.y};
    if (max_y < min_y) {
        auto temp{min_y};
        min_y = max_y;
        min_y = temp;
    }

    bottom_left = {min_x, min_y};
    top_right = {max_x, max_y};
};
