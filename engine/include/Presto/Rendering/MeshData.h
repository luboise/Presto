#pragma once

#include "Presto/Rendering/MaterialData.h"

namespace Presto {
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

        MaterialData material_data;

        int draw_mode{};
    };

}  // namespace Presto
