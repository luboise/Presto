#pragma once

namespace PrestoCore {
    struct RenderData {
        VertexList vertices;
        IndexList indices;

        [[nodiscard]] uint32_t getVertexCount() const {
            return this->vertices.size();
        };
    };
}  // namespace PrestoCore
