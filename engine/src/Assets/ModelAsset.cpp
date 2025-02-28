#include <utility>

#include "Presto/Assets/ModelAsset.h"

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
}  // namespace Presto
