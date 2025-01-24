#pragma once

#include "Presto/Core/Types.h"

#include "Presto/Rendering/Renderer.h"
#include "Presto/Resources/MaterialResource.h"

#include "Resource.h"

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

        MaterialResource* defaultMaterial_;

        int draw_mode{};
    };

    class MeshResource final : public Resource {
        friend class ResourceManager;
        friend class RenderingManager;

       public:
        [[nodiscard]] constexpr ResourceType getType() const override {
            return ResourceType::MESH;
        };

        [[nodiscard]] const std::vector<SubMesh>& getSubMeshes() const {
            return sub_meshes;
        }

       private:
        void load() override;

        std::vector<SubMesh> sub_meshes;

        render_data_id_t renderId_ = UNREGISTERED_RENDER_DATA_ID;
    };
}  // namespace Presto
