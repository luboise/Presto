#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Assets/Asset.h"

#include "Presto/Core/Constants.h"

#include "Presto/Rendering/BoundingBox.h"
#include "Presto/Rendering/RenderTypes.h"

#include "ImportTypes.h"

namespace Presto {

class MeshAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit MeshAsset();
    ~MeshAsset() override;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MESH;
    };

    MeshAsset& setVertices(const ImportedAttributeList& attributes);
    MeshAsset& setIndices(IndexList indices);

    MeshAsset& setDrawMode(MeshDrawMode mode);

    MeshAsset& setDefaultMaterial(const MaterialPtr&);

    // TODO: Adapt to imported mesh so it doesn't have to be calculated
    [[nodiscard]] BoundingBox getBoundingBox() const;  // namespace Presto

   private:
    struct Impl;

    Allocated<Impl> impl_;

    bool load() override;

    [[nodiscard]] bool modifiable() const;

    mesh_registration_id_t registrationId_{PR_UNREGISTERED};
    MaterialPtr defaultMaterial_;
};
}  // namespace Presto
