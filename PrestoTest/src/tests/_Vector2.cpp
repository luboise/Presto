#include "pch.h"

#include "include/Types.h"

namespace Vector2Test {
    TEST(DefaultConstructor, HasCorrectValues) {
        Vector2 v = Vector2();

        EXPECT_EQ(v.x, 0);
        EXPECT_EQ(v.y, 0);
    }
}  // namespace Vector2Test
