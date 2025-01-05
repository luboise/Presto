#pragma once

namespace Presto {
    struct PRESTO_API RenderData {
        VertexList vertices;
        IndexList indices;

        [[nodiscard]] uint32_t getVertexCount() const {
            return this->vertices.size();
        };
    };
}  // namespace Presto
