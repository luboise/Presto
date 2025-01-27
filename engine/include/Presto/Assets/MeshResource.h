#pragma once

#include "Presto/Core/Types.h"

#include "Presto/Rendering/MeshData.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Asset.h"

namespace Presto {
    class ModelAsset final : public Asset {
        friend class ResourceManager;
        friend class RenderingManager;

       public:
        explicit ModelAsset(resource_name_t name);

        [[nodiscard]] constexpr AssetType getType() const override {
            return AssetType::MESH;
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
