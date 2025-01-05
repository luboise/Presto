#pragma once

#include "Presto/Rendering/Renderer.h"

namespace Presto {
    struct MeshResource {
        friend class ResourceManager;

        PR_STRING_ID name;
        VertexList vertices;
        IndexList indices;
        render_data_id_t renderId_ = UNREGISTERED_RENDER_DATA_ID;

        MeshResource() = default;
        MeshResource(PR_STRING_ID name, VertexList vertices, IndexList indices)
            : name(std::move(name)),
              vertices(std::move(vertices)),
              indices(std::move(indices)) {}
    };
}  // namespace Presto
