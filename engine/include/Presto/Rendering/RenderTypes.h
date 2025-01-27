#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto {
    struct GlobalUniforms {
        Presto::mat4 view;
        Presto::mat4 projection;
    };

    struct ObjectUniforms {
        Presto::mat4 transform;
    };

    struct DrawInfo {
        size_t vertex_count = 0;

        // Vulkan uses signed int for the offset
        int32_t vertex_offset = 0;

        size_t index_count = 0;
        uint32_t index_offset = 0;
    };

    constexpr PR_NUMERIC_ID UNREGISTERED_RENDER_DATA_ID = -1;

    using renderer_mesh_id_t = PR_NUMERIC_ID;
    using renderer_material_id_t = PR_NUMERIC_ID;
    using renderer_texture_id_t = PR_NUMERIC_ID;

    struct VisualExtents {
        size_t width;
        size_t height;
    };

    struct MeshData {
        int draw_mode;
        VertexList vertices;
        IndexList indices;
    };

    enum class ShaderStage { VERTEX, FRAGMENT };

}  // namespace Presto
