#pragma once

struct Vertex {
    glm::vec3 pos;
    glm::vec3 colour;
};

using Index = uint32_t;

using VertexList = std::vector<Vertex>;
using IndexList = std::vector<Index>;

namespace Presto {
    struct PRESTO_API RenderData {
        VertexList vertices;
        IndexList indices;

        [[nodiscard]] uint32_t getVertexCount() const {
            return this->vertices.size();
        };
    };
}  // namespace Presto
