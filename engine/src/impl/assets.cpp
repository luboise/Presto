module presto.internal.assets;

import presto.internal.managers;
import presto.assets;

#include <utility>

namespace Presto {

Ptr<Texture2D> NewTexture2D(AssetArg path) {
    ImagePtr image{LoadImage(path)};

    if (image == nullptr) {
        PR_ERROR(
            "Unable to load image from path {}. Skipping Texture2D creation.",
            path.string());
        return nullptr;
    }

    return NewTexture2D(image);
};

Ptr<Texture2D> NewTexture2D(const ImagePtr& ptr) {
    return RenderingManager::get().createTexture2D(ptr);
};

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

Ptr<FontSource> CreateFontSource(const AssetArg& filepath) {
    return AssetManager::get().addAssetSource<FontSource>(filepath);
};

Presto::MaterialPtr Presto::NewMaterial(MaterialType type,
                                        Presto::string name) {
    using namespace Presto;

    return RenderingManager::get().createMaterial(type, std::move(name));
};

Presto::MaterialPtr Presto::FindMaterial(const Presto::string& name) {
    using namespace Presto;

    return RenderingManager::get().findMaterial(name);
};
}  // namespace Presto
