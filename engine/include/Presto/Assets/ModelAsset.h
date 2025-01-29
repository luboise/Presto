#pragma once

#include "Presto/Core/Types.h"

#include "Asset.h"

#include "Presto/Assets/MeshAsset.h"

#include "Presto/Rendering/BoundingBox.h"

namespace Presto {
    class ModelAsset final : public Asset {
        friend class AssetManager;
        friend class RenderingManager;

       public:
        using MeshPtr = AssetPtr<MeshAsset>;

        explicit ModelAsset(asset_name_t);

        [[nodiscard]] constexpr AssetType getType() const override {
            return AssetType::MODEL;
        };

        [[nodiscard]] const std::vector<MeshPtr>& getMeshes() {
            return meshes_;
        }

        BoundingBox getBoundingBox();

       private:
        void load() override;

        // std::vector<MeshData>

        // std::vector<RawMeshData> meshes_;
        // std::vector<MeshData> meshes_;
        std::vector<MeshPtr> meshes_;
    };

    using MeshPtr = ModelAsset::MeshPtr;
    using ModelPtr = AssetPtr<ModelAsset>;
}  // namespace Presto
