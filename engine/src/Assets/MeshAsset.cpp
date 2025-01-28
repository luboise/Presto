#include "Presto/Assets/MeshAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    MeshAsset::MeshAsset(MeshData&& data) : data_(std::move(data)) {}

    void MeshAsset::load() { RenderingManager::get().loadMeshOnGpu(*this); }

    void MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
        defaultMaterial_ = material;
    };
}  // namespace Presto
