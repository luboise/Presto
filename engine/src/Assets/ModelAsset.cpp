#include <utility>

#include "Presto/Assets/ModelAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ModelAsset::load() { RenderingManager::get().loadModelOnGpu(*this); };

    ModelAsset::ModelAsset(asset_name_t modelName)
        : Asset(std::move(modelName)) {}
}  // namespace Presto
