#pragma once

namespace Presto {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
    };

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<uint32_t>;
}  // namespace Presto
