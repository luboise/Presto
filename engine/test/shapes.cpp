
#include <gtest/gtest.h>

#include "Presto/Collision/Shapes.h"
#include "Presto/Math.h"
#include "Presto/Presto.h"

using namespace Presto;

TEST(CubeTests, VertexTests) {
    Cube c{vec3{0, 0, 0}};
    auto vertices{c.vertices()};
    ASSERT_EQ(vertices[0], vec3{-0.5, -0.5, -0.5});
}
