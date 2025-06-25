#include <gtest/gtest.h>

import presto;
import presto.math;

using namespace Pr;

TEST(QuaternionTests, AxisConstructor) {
    Quaternion q{180, {0, 1, 0}};
    ASSERT_NEAR(q.w, 0, NEAR_DISTANCE);
    ASSERT_NEAR(1, std::abs(q.xyz.y), NEAR_DISTANCE);

    q = Quaternion{45, {1, 0, 0}};
    ASSERT_NEAR(q.w, 0.92387, NEAR_DISTANCE);
    ASSERT_NEAR(std::sin(glm::radians(45.F) / 2), q.xyz.x, NEAR_DISTANCE);

    q = Quaternion{45, {0, 1, 0}};
    ASSERT_NEAR(q.w, 0.92387, NEAR_DISTANCE);
    ASSERT_NEAR(std::sin(glm::radians(45.F) / 2), q.xyz.y, NEAR_DISTANCE);

    q = Quaternion{45, {0, 0, 1}};
    ASSERT_NEAR(q.w, 0.92387, NEAR_DISTANCE);
    ASSERT_NEAR(std::sin(glm::radians(45.F) / 2), q.xyz.z, NEAR_DISTANCE);
}

TEST(QuaternionTests, PointMultiplication) {
    const Pr::vec3 point{1, 0, 0};

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

    q2 = Quaternion::fromEuler({45, 45, 45});
    ASSERT_NEAR(q2.w, 0.8446, NEAR_DISTANCE);
    ASSERT_NEAR(q2.xyz.x, 0.1913, NEAR_DISTANCE);
    ASSERT_NEAR(q2.xyz.y, 0.4619, NEAR_DISTANCE);
    ASSERT_NEAR(q2.xyz.z, 0.1913, NEAR_DISTANCE);
}

TEST(QuaternionTests, ToMat3) {
    const Pr::vec3 point{1, 0, 0};

    Quaternion q{90, {0, 1, 0}};
    vec3 quat_val = q * point;
    vec3 mat_val = q.toMat3() * point;
    ASSERT_NEAR(glm::length(quat_val - mat_val), 0, NEAR_DISTANCE);
}

TEST(QuaternionTests, ToEuler) {
    vec3 angles_in{45, 45, 45};

    auto q = Quaternion::fromEuler(angles_in);
    vec3 angles_out = q.toEuler();

    ASSERT_NEAR(angles_in.x, angles_out.x, NEAR_DISTANCE);
    ASSERT_NEAR(angles_in.y, angles_out.y, NEAR_DISTANCE);
    ASSERT_NEAR(angles_in.z, angles_out.z, NEAR_DISTANCE);
}
