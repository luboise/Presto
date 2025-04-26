#include "Modules/AssetManager.h"
#include "Presto/Assets.h"

namespace Presto {

ImagePtr LoadImage(const AssetArg& path, Presto::string name) {
    if (name.empty()) {
        name = path.basename();
    }

    return AssetManager::get().loadImageFromDisk(path, name);
}

/*
ModelPtr LoadModel(const AssetArg& filepath, const asset_name_t& customName) {
    ModelLoadResult result{
        AssetManager::get().loadModelsFromDisk(filepath, customName)};

    if (!result.models.empty()) {
        return result.models[0];
    }

    return nullptr;
}
*/

ModelPtr FindModel(const asset_name_t& name) {
    return AssetManager::get().find<AssetType::MODEL>(name);
};

Ptr<MeshSource> CreateMeshSource(const AssetArg& filepath) {
    return AssetManager::get().addAssetSource<MeshSource>(filepath);
}

}  // namespace Presto
