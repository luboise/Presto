#include <vector>

#include "presto/assets/mesh.h"
#include "presto/assets/types.h"
#include "presto/core.h"

namespace Presto {
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

}  // namespace Presto
