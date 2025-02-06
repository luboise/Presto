#include "Presto/Assets/MeshAsset.h"

#include "Modules/ModelLoader.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {
MeshAsset::MeshAsset(ImportedMesh&& mesh)
    : importedMesh_(std::move(mesh)), box_(this->getBoundingBox()) {}

void MeshAsset::load() { RenderingManager::get().loadMeshOnGpu(*this); }

void MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
    defaultMaterial_ = material;
};
}  // namespace Presto
