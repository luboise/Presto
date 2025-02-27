#pragma once

#include "Presto/Types/CoreTypes.h"

#include "Asset.h"

#include "Presto/Assets/MeshAsset.h"

#include "Presto/Rendering/BoundingBox.h"

namespace Presto {

class ModelAsset final : public Asset {
    friend class AssetManager;
    friend class RenderingManager;

   public:
    explicit ModelAsset(asset_name_t);

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MODEL;
    };

    [[nodiscard]] const std::vector<MeshPtr>& getMeshes() { return meshes_; }

    BoundingBox getBoundingBox();

   private:
    bool load() override;

    // std::vector<MeshData>

    // std::vector<RawMeshData> meshes_;
    // std::vector<MeshData> meshes_;
    std::vector<MeshPtr> meshes_;
};

}  // namespace Presto
