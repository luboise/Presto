#pragma once

#include "Module.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Assets/MeshAsset.h"
#include "Presto/Core/Types.h"

namespace Presto {
    // enum class ResourceType { JSON, RAW };

    template <AssetType Type>
    struct ResourceTraits;

    template <>
    struct ResourceTraits<AssetType::MESH> {
        using ResourceT = ModelAsset;
        using ResourcePtr = ResourceT*;
    };

    template <>
    struct ResourceTraits<AssetType::MATERIAL> {
        using ResourceT = MaterialAsset;
        using ResourcePtr = ResourceT*;
    };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void init();
        void update() override {}
        static void shutdown();

        std::vector<ModelAsset*> loadMeshesFromDisk(
            const AssetPath& path, const resource_name_t& customName);

        ImageAsset& loadImageFromDisk(const AssetPath& path,
                                      const resource_name_t& customName);

        MaterialAsset& createMaterial(const resource_name_t& customName);

        ImageAsset* createImageResource(const resource_name_t& customName,
                                        const Presto::Image& image);

        template <ResourceType Type>
        [[nodiscard]] ResourceTraits<Type>::ResourcePtr find(
            const resource_name_t& key) {
            return resources_[Type][key]
                ->as<typename ResourceTraits<Type>::ResourceT>();
        };

        // Deleted functions
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        ResourceManager() = default;

        std::map<ResourceType,
                 std::map<resource_name_t, std::unique_ptr<Asset>>>
            resources_;

        /*
// Gets the type of name from the MeshResource struct
std::map<resource_name_t, std::unique_ptr<MeshResource>> meshResources_;
std::map<resource_name_t, std::unique_ptr<ImageResource>>
    imageResources_;
std::map<resource_name_t, std::unique_ptr<MaterialResource>>
    materialResources_;
*/
    };
}  // namespace Presto
