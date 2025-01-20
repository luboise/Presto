#pragma once

#include "Presto/Core/Types.h"

#include "Presto/Rendering/Renderer.h"
#include "Presto/Resources/MaterialResource.h"

namespace Presto {
    struct SubMesh {
        using PositionType = vec3;
        using NormalType = vec3;
        using TexCoordsType = vec2;
        using IndexType = unsigned short;

        std::vector<PositionType> positions;
        std::vector<NormalType> normals;
        std::vector<TexCoordsType> tex_coords;

        // size_t index_count;
        std::vector<IndexType> indices;

        MaterialResource* material;

        int draw_mode{};
    };

    struct MeshResource {
        friend class ResourceManager;

        PR_STRING_ID name;
        std::vector<SubMesh> sub_meshes;

        render_data_id_t renderId_ = UNREGISTERED_RENDER_DATA_ID;

        [[nodiscard]] bool isRegistered() const {
            return renderId_ != UNREGISTERED_RENDER_DATA_ID;
        }
    };
}  // namespace Presto
