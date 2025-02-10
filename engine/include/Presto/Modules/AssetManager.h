#pragma once

#include "Module.h"

// #include "Presto/Assets/ImageAsset.h"
// #include "Presto/Assets/MaterialAsset.h"
// #include "Presto/Assets/ModelAsset.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/MaterialInstanceAsset.h"
#include "Presto/Assets/ModelAsset.h"

#include "Presto/Core/Types.h"
#include "Presto/Rendering/MaterialTypes.h"

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
struct AssetTraits<AssetType::MATERIAL> {
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

    std::vector<ModelPtr> loadModelsFromDisk(const AssetPath& filepath,
                                             const asset_name_t& customName) {
        return loadModelsFromDisk(filepath, std::vector{customName});
    };

    std::vector<ModelPtr> loadModelsFromDisk(
        const AssetPath& filepath,
        const std::vector<asset_name_t>& customNames = {});

    ImagePtr loadImageFromDisk(const AssetPath& path,
                               const asset_name_t& customName);

    MaterialPtr createMaterial(const asset_name_t& customName,
                               MaterialStructure structure);

    template <MaterialType T>
        requires requires { typename MaterialTypeTraits<T>; } &&
                 requires { typename MaterialTypeTraits<T>::property_list; }
    MaterialTypeTraits<T>::property_list createMaterialInstance(
        const asset_name_t& customName, const MaterialPtr& material);

    template <>
    MaterialTypeTraits<MaterialType::DEFAULT_3D> createMaterialInstance(
        const asset_name_t& customName, const MaterialPtr& material) {
        auto new_instance{
            std::make_shared<MaterialInstanceAsset>(customName, material)};
        assets_[AssetType::MATERIAL_INSTANCE][customName] = new_instance;

        return new_instance;
    };

    ImagePtr createImageAsset(const asset_name_t& customName,
                              const Presto::Image& image);

    template <AssetType Type>
    [[nodiscard]] AssetTraits<Type>::ResourcePtr find(const asset_name_t& key) {
        using ReturnType = typename AssetTraits<Type>::ResourceT;
        return std::dynamic_pointer_cast<ReturnType>(assets_[Type][key]);
    };

   private:
    AssetManager() = default;
    ~AssetManager() = default;

    std::map<AssetType, std::map<asset_name_t, std::shared_ptr<Asset>>> assets_;
};

template <>
MaterialTypeTraits<MaterialType::DEFAULT_3D>::property_list
AssetManager::createMaterialInstance<MaterialType::DEFAULT_3D>(
    const asset_name_t& customName, const MaterialPtr& material);

}  // namespace Presto
