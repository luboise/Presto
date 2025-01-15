#pragma once

#include "Module.h"

#include "Presto/Core/Types.h"
#include "Presto/Resources/ImageResource.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void Init();
        void Update() override {}

        MeshResource& loadMeshFromDisk(const AssetPath& path,
                                       const resource_name_t& customName);

        ImageResource& loadImageFromDisk(const AssetPath& path,
                                         const resource_name_t& customName);

        [[nodiscard]] MeshResource* getMesh(const resource_name_t&) const;
        [[nodiscard]] ImageResource* getImage(const resource_name_t&) const;

        // Deleted functions
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        ResourceManager() = default;

        // Gets the type of name from the MeshResource struct
        std::map<resource_name_t, std::unique_ptr<MeshResource>> meshResources_;
        std::map<resource_name_t, std::unique_ptr<ImageResource>>
            imageResources_;
    };
}  // namespace Presto
