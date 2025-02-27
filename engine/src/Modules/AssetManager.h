#pragma once

#include "Module.h"

// #include "Presto/Assets/ImageAsset.h"
// #include "Presto/Assets/ModelAsset.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Assets/ModelAsset.h"

#include "Presto/Types/AssetTypes.h"
#include "Presto/Types/CoreTypes.h"

#include <map>

namespace Presto {
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

class PRESTO_API AssetManager final : public Module<AssetManager> {
    MODULE_FUNCTIONS(AssetManager);

   public:
    void update() override {}

    /**
     * @brief Loads models from the disk, and returns a list of loaded models.
     */
    std::vector<ModelPtr> loadModelsFromDisk(const AssetArg& filepath,
                                             const asset_name_t& customName) {
        return loadModelsFromDisk(filepath, std::vector{customName});
    };

    std::vector<ModelPtr> loadModelsFromDisk(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames = {});

    ImagePtr loadImageFromDisk(const AssetArg& path,
                               const asset_name_t& customName);

    ImagePtr createImageAsset(const asset_name_t& customName,
                              const Presto::Image& image);

    template <AssetType Type>
    [[nodiscard]] auto find(const asset_name_t& key)
        -> AssetTraits<Type>::ResourcePtr {
        using ReturnType = typename AssetTraits<Type>::ResourceT;
        return std::dynamic_pointer_cast<ReturnType>(assets_[Type][key]);
    };

    Ptr<MaterialAsset> getMaterialDefinition(pipeline_id_t id);

   private:
    AssetManager() = default;
    ~AssetManager() override = default;

    std::map<AssetType, std::map<asset_name_t, std::shared_ptr<Asset>>> assets_;

    MaterialPtr createMaterialFromImport(const ImportedMaterial&,
                                         std::vector<Ptr<Texture>>&);
};

/**/
/*template <>*/
/*MaterialTypeTraits<MaterialType::DEFAULT_3D>::property_list*/
/*AssetManager::createMaterialInstance<MaterialType::DEFAULT_3D>(*/
/*    const asset_name_t& customName, const MaterialPtr& material);*/
/**/

}  // namespace Presto
