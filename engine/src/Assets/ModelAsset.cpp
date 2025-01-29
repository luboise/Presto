#include <utility>

#include "Presto/Assets/ModelAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ModelAsset::load() { RenderingManager::get().loadModelOnGpu(*this); };

    ModelAsset::ModelAsset(asset_name_t modelName)
        : Asset(std::move(modelName)) {}

    BoundingBox ModelAsset::getBoundingBox() {
        BoundingBox box{};

        for (const auto& mesh : meshes_) {
            box.merge(mesh->getBoundingBox());
        }

        return box;
    }
}  // namespace Presto
