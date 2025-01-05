#pragma once

#include <vector>
#include "PrestoCore/Math.h"

namespace PrestoCore {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 colour;
    };

    using Index = uint32_t;

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<Index>;
}  // namespace PrestoCore
