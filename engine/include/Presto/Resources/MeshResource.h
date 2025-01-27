#pragma once

#include "Presto/Core/Types.h"

#include "Presto/Rendering/MeshData.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Resource.h"

namespace Presto {
    class MeshResource final : public Resource {
        friend class ResourceManager;
        friend class RenderingManager;

       public:
        explicit MeshResource(resource_name_t name);

        [[nodiscard]] constexpr ResourceType getType() const override {
            return ResourceType::MESH;
        };

        [[nodiscard]] const std::vector<RawMeshData>& getSubMeshes() const {
            return meshes_;
        }

       private:
        void load() override;

        std::vector<RawMeshData> meshes_;
        std::vector<renderer_mesh_id_t> meshIds_;
        std::vector<renderer_material_id_t> materialIds_;
    };
}  // namespace Presto
