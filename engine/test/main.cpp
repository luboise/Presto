#include <gtest/gtest.h>

#include "Presto/Collision/Shapes.h"
#include "Presto/Core/Logging.h"
#include "Presto/Presto.h"

using namespace Presto;

TEST(RectangleCollisionTests, BasicAssertions) {
    Rectangle rec({-2, -4}, {2, 4});

    LineSegment2D seg{.p1{-10, -3.5}, .p2{0, 0}};

    ASSERT_TRUE(Presto::Intersects2D(rec, seg));

    seg.p2 = {-2, 0};
    ASSERT_TRUE(Presto::Intersects2D(rec, seg));

    seg.p2 = {-2.1, 0};
    ASSERT_FALSE(Presto::Intersects2D(rec, seg));
}

TEST(RectangleCollisionTests, SimpleCases) {
    LineSegment2D seg{.p1{-3, 0}, .p2{-0.4, 0}};
    Rectangle rec({-0.5, -1}, {0.5, 1});

    ASSERT_TRUE(Presto::Intersects2D(rec, seg));
}

// Skewer cases represent cases where the neither point is inside of the
// rectangle, and the edge collision skewers the rectangle
TEST(RectangleCollisionTests, SkewerCases) {
    LineSegment2D seg{.p1{-3, 0}, .p2{3, 0}};
    Rectangle rec({-0.5, -1}, {0.5, 1});

    ASSERT_TRUE(Presto::Intersects2D(rec, seg));
}

TEST(CylinderCollisionTests, BasicAssertions) {
    Presto::Log::init();

    Cylinder c{.radius = 0.5, .height = 1};
    Triangle tri{.p1{-0.4, 0, 0}, .p2{-3, 0, 0}, .p3{-4, 5, 2}};
    ASSERT_TRUE(Intersects(c, tri));

    tri.p1 = {-0.5, 0, 0};
    ASSERT_TRUE(Intersects(c, tri));

    tri.p1 = {-0.6, 0, 0};
    ASSERT_FALSE(Intersects(c, tri));
}
