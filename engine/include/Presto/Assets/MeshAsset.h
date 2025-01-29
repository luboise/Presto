#pragma once

#include "Presto/Assets/Asset.h"
#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Rendering/BoundingBox.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    class MeshAsset final : public Asset {
        friend class RenderingManager;

       public:
        explicit MeshAsset(MeshData&&);

        [[nodiscard]] constexpr AssetType getType() const override {
            return AssetType::MESH;
        };

        [[nodiscard]] inline BoundingBox getBoundingBox() const {
            return data_.getBoundingBox();
        };

        void setDefaultMaterial(const MaterialPtr&);

       private:
        MeshData data_{};

        void load() override;

        renderer_mesh_id_t renderId_{UNREGISTERED_RENDER_DATA_ID};
        std::weak_ptr<MaterialAsset> defaultMaterial_;
    };
}  // namespace Presto
