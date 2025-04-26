#include <utility>

#include "Modules/RenderingManager.h"
#include "Presto/Assets/ModelAsset.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/MeshData.h"

namespace Presto {
bool ModelAsset::load() { return true; };

ModelAsset::ModelAsset(asset_name_t modelName) : Asset(std::move(modelName)) {}

BoundingBox ModelAsset::getBoundingBox() {
    BoundingBox box{};

    for (const auto& mesh : meshes_) {
        box.merge(mesh->getBoundingBox());
    }

    return box;
}

ModelAsset& ModelAsset::addMesh(MeshData meshData, MaterialPtr newMaterial) {
    meshData.pipeline_id = PR_PIPELINE_DEFAULT_3D;
    auto registration_id = RenderingManager::get().loadMesh(meshData);

    draws_.push_back(MeshDraw{.mesh{meshData}, .material{newMaterial}});
    meshes_.push_back(newMesh);
    return *this;
};

ModelAsset& ModelAsset::clear() {
    draws_.clear();

    return *this;
}

}  // namespace Presto
