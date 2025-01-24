#pragma once

#include "Module.h"

#include "Presto/Core/Types.h"
#include "Presto/Resources/ImageResource.h"
#include "Presto/Resources/MaterialResource.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    // enum class ResourceType { JSON, RAW };

    template <ResourceType Type>
    struct ResourceTraits;

    template <>
    struct ResourceTraits<ResourceType::MESH> {
        using ResourcePtr = MeshResource*;
    };

    template <>
    struct ResourceTraits<ResourceType::MATERIAL> {
        using ResourcePtr = MaterialResource*;
    };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void init();
        void update() override {}
        static void shutdown();

        std::vector<MeshResource*> loadMeshesFromDisk(
            const AssetPath& path, const resource_name_t& customName);

        ImageResource& loadImageFromDisk(const AssetPath& path,
                                         const resource_name_t& customName);

        MaterialResource& createMaterial(const resource_name_t& customName);

        template <ResourceType Type>
        [[nodiscard]] ResourceTraits<Type>::ResourcePtr find(
            const resource_name_t& key) {
            return resources_[Type][key].get();
        };

        // Deleted functions
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        ResourceManager() = default;

        std::map<ResourceType,
                 std::map<resource_name_t, std::unique_ptr<Resource>>>
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
