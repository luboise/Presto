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
    explicit MeshAsset();

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MESH;
    };

    MeshAsset& setVertices(const AttributeList& attributes);
    MeshAsset& setIndices(const IndexList& attributes);

    MeshAsset& setDefaultMaterial(const MaterialPtr&);

    // TODO: Adapt to imported mesh so it doesn't have to be calculated
    [[nodiscard]] BoundingBox getBoundingBox() const;  // namespace Presto

   private:
    struct Impl;

    Allocated<Impl> impl_;

    bool load() override;

    [[nodiscard]] bool modifiable() const;

    renderer_mesh_id_t renderId_{UNREGISTERED_RENDER_DATA_ID};
    Ref<MaterialAsset> defaultMaterial_;
};
}  // namespace Presto
