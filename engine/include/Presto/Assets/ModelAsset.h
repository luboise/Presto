#pragma once

#include "Presto/Types/CoreTypes.h"

#include "Asset.h"

#include "Presto/Assets/MeshAsset.h"

#include "Presto/Rendering/BoundingBox.h"

namespace Presto {

class MeshSource;
struct MeshDraw;

class ModelAsset final : public Asset {
    friend class AssetManager;
    friend class RenderingManager;

   public:
    explicit ModelAsset(asset_name_t);

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MODEL;
    };

    [[nodiscard]] Presto::size_t meshCount() const;
    [[nodiscard]] const std::vector<MeshDraw>& getDraws() { return draws_; }

    BoundingBox getBoundingBox();

    ModelAsset& addMesh(MeshPtr, MaterialPtr = nullptr);

    ModelAsset& clear();

   private:
    bool load() override;

    // std::vector<MeshData>

    // std::vector<RawMeshData> meshes_;
    // std::vector<MeshData> meshes_;

    std::vector<MeshDraw> draws_;
    Ptr<MeshSource> source_;
};

}  // namespace Presto
