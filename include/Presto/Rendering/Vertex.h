#pragma once

namespace Presto {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
    };

    using Index = uint32_t;

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<Index>;
}  // namespace Presto
