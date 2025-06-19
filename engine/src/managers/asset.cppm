export module presto.internal.managers.asset;

import presto.assets;
import presto.internal;

export namespace Presto {
// enum class ResourceType { JSON, RAW };

template <AssetType Type>
struct AssetTraits;

template <>
struct AssetTraits<AssetType::MESH> {
    using ResourceT = ModelAsset;
    using ResourcePtr = AssetPtr<ResourceT>;
};

template <>
struct AssetTraits<AssetType::MATERIAL_DEFINITION> {
    using ResourceT = MaterialAsset;
    using ResourcePtr = AssetPtr<ResourceT>;
};

template <>
struct AssetTraits<AssetType::IMAGE> {
    using ResourceT = ImageAsset;
    using ResourcePtr = AssetPtr<ResourceT>;
};

template <>
struct AssetTraits<AssetType::MODEL> {
    using ResourceT = ModelAsset;
    using ResourcePtr = AssetPtr<ResourceT>;
};

struct ModelLoadResult {
    bool success;

    std::vector<ModelPtr> models;
    std::vector<TexturePtr> textures;
    std::vector<MaterialPtr> materials;
};

class PRESTO_API AssetManager final : public Module<AssetManager> {
    MODULE_FUNCTIONS(AssetManager);

   public:
    void update() override {}

    /**
     * @brief Loads models from the disk, and returns all newly loaded assets
     */
    /*
ModelLoadResult loadModelsFromDisk(const AssetArg& filepath,
                                   const asset_name_t& customName) {
    return loadModelsFromDisk(filepath, std::vector{customName});
};

ModelLoadResult loadModelsFromDisk(
    const AssetArg& filepath,
    const std::vector<asset_name_t>& customNames = {});
            */

    ImagePtr loadImageFromDisk(const AssetArg& path,
                               const asset_name_t& customName);

    template <AssetType Type>
    [[nodiscard]] auto find(const asset_name_t& key)
        -> AssetTraits<Type>::ResourcePtr {
        using ReturnType = typename AssetTraits<Type>::ResourceT;
        return std::dynamic_pointer_cast<ReturnType>(assets_[Type][key]);
    };

    Ptr<MaterialAsset> getMaterialDefinition(pipeline_id_t id);

    MaterialDefinitionPtr createMaterialDefinition(
        Presto::string name, const PipelineStructure& structure);

    template <typename T>
        requires DerivedFrom<T, AssetSource, Strictness::STRICTLY_DERIVED>
    Ptr<T> addAssetSource(const AssetArg& filepath) {
        Ptr<T> new_source{std::make_shared<T>(filepath)};

        assetSources_.push_back(
            std::static_pointer_cast<AssetSource>(new_source));

        return new_source;
    };

    /*
MaterialPtr createMaterialFromImport(const ImportedMaterial&,
                                     std::vector<Ptr<Texture>>&);
                                                                             */

    template <DerivedFrom<Asset> T, typename... Args>
        requires std::is_constructible_v<T, Args...>
    Ptr<T> newAsset(Args&&... args) {
        Ptr<T> new_asset{new T(std::forward<Args>(args)...)};
        addAsset(new_asset);

        return new_asset;
    }

    // TODO: Make this check for existing assets before adding
    template <typename T>
        requires DerivedFrom<T, Asset>
    void addAsset(Ptr<T> asset) {
        assets_[asset->type()][asset->name()] = asset;
    }

   private:
    AssetManager() = default;
    ~AssetManager() override = default;

    std::map<AssetType, std::map<asset_name_t, std::shared_ptr<Asset>>> assets_;

    std::vector<Ptr<AssetSource>> assetSources_;
};

/**/
/*template <>*/
/*MaterialTypeTraits<MaterialType::DEFAULT_3D>::property_list*/
/*AssetManager::createMaterialInstance<MaterialType::DEFAULT_3D>(*/
/*    const asset_name_t& customName, const MaterialPtr& material);*/
/**/

}  // namespace Presto
