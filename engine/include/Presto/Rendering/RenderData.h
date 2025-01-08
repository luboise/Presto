#pragma once

namespace Presto {

    struct RenderData {
        int draw_mode;
        VertexList vertices;
        IndexList indices;
    };

    struct GLTFRenderData {
        std::vector<std::byte> buffer;

        size_t vertex_count;
        size_t vertex_offset;

        size_t index_count;
        size_t index_offset;

        int open_gl_draw_mode;
    };

}  // namespace Presto
