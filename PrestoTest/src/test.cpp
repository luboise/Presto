#include "pch.h"

#include "Presto/Common/Types.h"

TEST(DefaultConstructor, HasCorrectValues) {
    EXPECT_EQ(1, 1);

    Vector2 v = Vector2();

    EXPECT_EQ(v.x, 0);
    EXPECT_EQ(v.y, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}