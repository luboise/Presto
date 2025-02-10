#pragma once

#include "Presto/Assets/Asset.h"
#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Rendering/BoundingBox.h"
#include "Presto/Rendering/RenderTypes.h"

#include "ImportTypes.h"

namespace Presto {

class MeshAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit MeshAsset(ImportedMesh&&);

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MESH;
    };

    // TODO: Adapt to imported mesh
    [[nodiscard]] BoundingBox getBoundingBox() const {
        return {.x_min = -1,
                .x_max = 1,
                .y_min = -1,
                .y_max = 1,
                .z_min = -1,
                .z_max = 1};

        //        return importedMesh_.getBoundingBox();
    };  // namespace Presto

    void setDefaultMaterial(const MaterialPtr&);

   private:
    ImportedMesh importedMesh_;
    BoundingBox box_;

    void load() override;

    renderer_mesh_id_t renderId_{UNREGISTERED_RENDER_DATA_ID};
    Ref<MaterialAsset> defaultMaterial_;
};
}  // namespace Presto
