#pragma once

namespace Presto {
    struct GlobalUniforms {
        Presto::mat4 view;
        Presto::mat4 projection;
    };

    struct ObjectUniforms {
        Presto::mat4 transform;
    };

    struct DrawInfo {
        std::size_t vertex_count = 0;

        // Vulkan uses signed int for the offset
        int32_t vertex_offset = 0;

        std::size_t index_count = 0;
        uint32_t index_offset = 0;
    };

    constexpr PR_NUMERIC_ID UNREGISTERED_RENDER_DATA_ID = -1;

    using renderer_mesh_id_t = PR_NUMERIC_ID;
    using renderer_pipeline_id_t = PR_NUMERIC_ID;
    using renderer_texture_id_t = PR_NUMERIC_ID;

    struct RawMeshData {
        using PositionType = vec3;
        using NormalType = vec3;
        using TexCoordsType = vec2;
        using IndexType = unsigned short;

        std::vector<PositionType> positions;
        std::vector<NormalType> normals;
        std::vector<TexCoordsType> tex_coords;

        // size_t index_count;
        std::vector<IndexType> indices;

        int draw_mode{};
    };

    struct MeshData {
        int draw_mode;
        VertexList vertices;
        IndexList indices;

        static MeshData from(const RawMeshData&);
    };

    enum class ShaderStage { VERTEX, FRAGMENT };

}  // namespace Presto
