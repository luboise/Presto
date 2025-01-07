#pragma once

#include "Presto/Rendering/Renderer.h"

namespace Presto {
    struct MeshResource {
        friend class ResourceManager;

        PR_STRING_ID name;

        size_t vertex_count;

        using PositionType = vec3;
        using NormalType = vec3;
        using TexCoordsType = vec2;
        using IndexType = uint32_t;

        std::vector<PositionType> positions;
        std::vector<NormalType> normals;
        std::vector<TexCoordsType> tex_coords;

        size_t index_count;
        std::vector<IndexType> indices;

        int draw_mode{};

        render_data_id_t renderId_ = UNREGISTERED_RENDER_DATA_ID;

        [[nodiscard]] bool isRegistered() const {
            return renderId_ != UNREGISTERED_RENDER_DATA_ID;
        }
    };
}  // namespace Presto
