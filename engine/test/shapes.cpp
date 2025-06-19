
#include <gtest/gtest.h>

import presto.math;

using namespace Presto;

TEST(CubeTests, VertexTests) {
    Cube c{vec3{0, 0, 0}};
    auto vertices{c.vertices()};
    vec3 target{-0.5, -0.5, -0.5};
    ASSERT_EQ(vertices[0], target);
}
