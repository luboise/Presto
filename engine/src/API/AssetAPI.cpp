#include "Modules/AssetManager.h"
#include "Presto/Assets.h"

namespace Presto {

ImagePtr LoadImage(const AssetArg& path) {
    // TODO: Add naming
    return AssetManager::get().loadImageFromDisk(path, "UnnamedImage");
}

ModelPtr LoadModel(const AssetArg& filepath, const asset_name_t& customName) {
    auto models{AssetManager::get().loadModelsFromDisk(filepath, customName)};
    if (!models.empty()) {
        return models[0];
    }

    return nullptr;
}

ModelPtr FindModel(const asset_name_t& name) {
    return AssetManager::get().find<AssetType::MODEL>(name);
};

}  // namespace Presto
