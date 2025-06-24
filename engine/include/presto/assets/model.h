#pragma once

#include <vector>

#include "presto/assets/asset.h"
#include "presto/assets/mesh.h"
#include "presto/core.h"

namespace Pr {
struct MeshDraw;

class ModelAsset final : public Asset {
    friend class AssetManager;
    friend class RenderingManager;

   public:
    explicit ModelAsset(asset_name_t);

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MODEL;
    };

    [[nodiscard]] Pr::size_t meshCount() const;
    [[nodiscard]] const std::vector<MeshDraw>& getDraws() { return draws_; }

    BoundingBox getBoundingBox();

    ModelAsset& addMesh(AssetPtr<MeshAsset>, AssetPtr<MaterialAsset> = nullptr);

    ModelAsset& clear();

   private:
    bool load() override;

    // std::vector<MeshData>

    // std::vector<RawMeshData> meshes_;
    // std::vector<MeshData> meshes_;

    std::vector<MeshDraw> draws_;
    Ptr<MeshSource> source_;
};

}  // namespace Pr
