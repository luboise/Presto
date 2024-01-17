#include "pch.h"

#include "include/Types.h"

TEST(DefaultConstructor, HasCorrectValues) {
    Vector3 v;

    EXPECT_EQ(v.x, 0);
    EXPECT_EQ(v.y, 0);
}

TEST(Constructors, InitialiseCorrectly) {
    EXPECT_EQ(Vector3(2, -5), Vector3(2, -5, 0));
    EXPECT_EQ(Vector3(), Vector3(0, 0, 0));
    EXPECT_FALSE(Vector3(2, 2) == Vector3(2, 2, 2));
}

TEST(GetLength, ReturnsZeroWhenZeroAppears) {
    Vector3 v;

    v = Vector3(0, 0, 0);
    EXPECT_EQ(v.getLength(), 0.0);
}

TEST(GetLength, RecognisesIdentityVectors) {
    Vector3 v(0, 1);

    EXPECT_FLOAT_EQ(v.getLength(), 1.0f);

    v = Vector3(-1, 0);
    EXPECT_FLOAT_EQ(v.getLength(), 1.0f);
}

TEST(GetLength, CalculatesLengthsCorrectly) {
    Vector3 v(3, 4);
    EXPECT_FLOAT_EQ(v.getLength(), 5.0f);

    v = Vector3(-3, 4);
    EXPECT_FLOAT_EQ(v.getLength(), 5.0f);

    v = Vector3(1, -1, 1);
    EXPECT_FLOAT_EQ(v.getLength(), sqrt(3));
}

TEST(EqualsEqualsVec, ComparesCorrectly) {
    Vector3 v1(1, 2);
    Vector3 v2(2, 1);
    EXPECT_FALSE(v1 == v2);

    v2.x = 1;
    v2.y = 2;
    EXPECT_TRUE(v1 == v2);

    v1 = Vector3(0, 0);
    v2 = Vector3(0, 0);
    EXPECT_TRUE(v1 == v2);
}

TEST(EqualsEqualsFloat, ComparesCorrectly) {
    Vector3 v1(3, 4);
    EXPECT_TRUE(v1 == 5.0f);
}

TEST(normalised, ReturnsCorrectValues) {
    Vector3 v1(5, 0, 0);
    Vector3 v2 = v1.normalised();

    EXPECT_FLOAT_EQ(v2.getLength(), 1.0f);
    EXPECT_EQ(v1.normalised(), Vector3(1, 0, 0));
}

TEST(normalised, ReturnsZeroIfZeroLength) {
    Vector3 v;

    EXPECT_FLOAT_EQ(v.normalised().getLength(), 1.0f);
}