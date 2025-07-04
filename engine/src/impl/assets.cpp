module presto.internal.assets;

import presto.internal.managers;
import presto.assets;

import std;

namespace Pr {

Ptr<Texture2D> NewTexture2D(AssetArg path) {
    Pr::Ptr<Pr::ImageAsset> image{LoadImage(path)};

    if (image == nullptr) {
        Pr::Log(
            ERROR,
            "Unable to load image from path {}. Skipping Texture2D creation.",
            path.string());
        return nullptr;
    }

    return NewTexture2D(image);
};

Ptr<Texture2D> NewTexture2D(const Pr::Ptr<Pr::ImageAsset>& ptr) {
    return RenderingManager::get().createTexture2D(ptr);
};

Pr::Ptr<Pr::ImageAsset> LoadImage(const AssetArg& path, Pr::string name) {
    if (name.empty()) {
        name = path.basename();
    }

    return AssetManager::get().loadImageFromDisk(path, name);
}

/*
Pr::Ptr<Pr::ModelAsset> LoadModel(const AssetArg& filepath, const asset_name_t&
customName) { ModelLoadResult result{
        AssetManager::get().loadModelsFromDisk(filepath, customName)};

    if (!result.models.empty()) {
        return result.models[0];
    }

    return nullptr;
}
*/

Pr::Ptr<Pr::ModelAsset> FindModel(const asset_name_t& name) {
    return AssetManager::get().find<AssetType::MODEL>(name);
};

Ptr<MeshSource> CreateMeshSource(const AssetArg& filepath) {
    return AssetManager::get().addAssetSource<MeshSource>(filepath);
}

Ptr<FontSource> CreateFontSource(const AssetArg& filepath) {
    return AssetManager::get().addAssetSource<FontSource>(filepath);
};

Pr::Ptr<Pr::MaterialInstance> Pr::NewMaterial(MaterialType type,
                                              Pr::string name) {
    using namespace Pr;

    return RenderingManager::get().createMaterial(type, std::move(name));
};

Pr::Ptr<Pr::MaterialInstance> Pr::FindMaterial(const Pr::string& name) {
    using namespace Pr;

    return RenderingManager::get().findMaterial(name);
};
}  // namespace Pr
