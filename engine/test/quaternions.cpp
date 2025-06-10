#include <gtest/gtest.h>

#include "Presto/Presto.h"

#include "Presto/Math.h"

using namespace Presto;

TEST(QuaternionTests, AxisConstructor) {
    Quaternion q{180, {0, 1, 0}};
    ASSERT_NEAR(q.w, 0, NEAR_DISTANCE);
    ASSERT_NEAR(1, std::abs(q.xyz.y), NEAR_DISTANCE);
}

TEST(QuaternionTests, PointMultiplication) {
    const Presto::vec3 point{1, 0, 0};

    Quaternion q{180, {0, 1, 0}};
    vec3 rotated = q * point;
    vec3 diff{rotated - vec3{-1, 0, 0}};
    ASSERT_LT(glm::length(diff), 0.1);

    // Check for 90 degrees around x axis
    q = Quaternion{90, {0, 1, 0}};
    rotated = q * point;
    diff = rotated - vec3{0, 0, -1};
    ASSERT_EQ(rotated.x, 0);
    ASSERT_EQ(rotated.y, 0);

    ASSERT_NEAR(rotated.z, -1, NEAR_DISTANCE);
}

TEST(QuaternionTests, FromEuler) {
    Quaternion q1{90, {0, 1, 0}};
    auto q2 = Quaternion::fromEuler({0, 90, 0});

    ASSERT_EQ(q1.w, q2.w);
    ASSERT_EQ(q1.xyz.x, q2.xyz.x);
    ASSERT_EQ(q1.xyz.y, q2.xyz.y);
    ASSERT_EQ(q1.xyz.z, q2.xyz.z);
}

TEST(QuaternionTests, ToMat3) {
    const Presto::vec3 point{1, 0, 0};

    Quaternion q{90, {0, 1, 0}};
    vec3 quat_val = q * point;
    vec3 mat_val = q.toMat3() * point;
    ASSERT_NEAR(glm::length(quat_val - mat_val), 0, NEAR_DISTANCE);
}
